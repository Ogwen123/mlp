#include "data.h"
#include "../utils.h"

static inline int COLUMN_PADDING = 1;

void Dataframe::from_csv_format(std::deque<std::string> lines, bool parse_column_names) {
	if (lines.size() == 0) {
		throw std::runtime_error("Dataframe::from_csv: Empty data");
	}

	std::vector<std::string> column_names;
	std::vector<std::vector<std::string>> columns;

	size_t expected_length = 0;

	if (parse_column_names) {
		column_names = Utils::split(lines.front());
		lines.pop_front();

		expected_length = column_names.size();
	}
	else {
		expected_length = Utils::split(lines.front()).size();
	}
	columns.resize(expected_length);

	for (; !lines.empty(); lines.pop_front()) {
		std::vector<std::string> split = Utils::split(lines.front());
		if (split.size() != expected_length) {
			throw std::runtime_error("Dataframe::from_csv: Input row size error");
		}
		for (int i = 0; i < expected_length; i++) {
			columns[i].push_back(split[i]);
		}
	}
	if (!parse_column_names) {
		for (int i = 0; i < columns[0].size(); i++) {
			column_names.push_back(std::format("col{}", i));
		}
	}

	this->column_names = column_names;
	std::vector<TypedColumn> typed_columns;

	for (auto col : columns) {
		typed_columns.push_back(this->choose_column_type(col));
	}
	this->data = typed_columns;
}

TypedColumn Dataframe::choose_column_type(std::vector<std::string> column) {
	enum type {
		boolean,
		integer,
		_double,
		string
	};
	std::string boolean_strings[4] = { "t", "f", "true", "false" };

	enum type highest = boolean;

	for (auto item : column) {
		std::string lower;
		// make lowercase for easier boolean checking
		std::transform(item.begin(), item.end(), std::back_inserter(lower),
			[](unsigned char c) { return std::tolower(c); });

		// try boolean if not already past it
		if (highest <= boolean && std::find(std::begin(boolean_strings), std::end(boolean_strings), item) != std::end(boolean_strings)) {
			highest = boolean;
			continue;
		}

		// try integer
		if (highest <= integer) {
			try {
				size_t idx;
				long long int i = std::stoll(item, &idx);
				if (idx < item.size()) { // means stoll() stopped before the end of the string, so the string as a whole is not a valid int
					highest = _double;
					continue;
				}
				else {
					highest = integer;
					continue;
				}
			}
			catch (std::invalid_argument err) {
				highest = _double;
				continue;
			}
		}

		// try double if not already past it
		if (highest <= _double) {
			try {
				size_t idx;
				double d = std::stod(item, &idx);
				if (idx < item.size()) { // means stod() stopped before the end of the string, so the string as a whole is not a valid int
					highest = string;
					continue;
				}
				else {
					highest = _double;
					continue;
				}
			}
			catch (std::invalid_argument err) {
				highest = string;
				continue;
			}
		}
	}

	switch (highest) {
	case boolean:
	{
		std::vector<bool> bools;
		bools.reserve(column.size());

		std::transform(column.begin(), column.end(), std::back_inserter(bools), [](std::string item) {
			if (item.size() == 0) {
				return false;
			}
			else {
				return (item[0] == 't' || item[0] == 'T');
			}
			});

		return bools;
	}
	case integer:
	{
		std::vector<int> ints;
		ints.reserve(column.size());

		std::transform(column.begin(), column.end(), std::back_inserter(ints), [](std::string item) {
			return std::stoi(item);
			});

		return ints;
	}
	case _double:
	{
		std::vector<double> doubles;
		doubles.reserve(column.size());

		std::transform(column.begin(), column.end(), std::back_inserter(doubles), [](std::string item) {
			return std::stod(item);
			});

		return doubles;
	}
	default:
		return column;
	}
}

void Dataframe::load_csv(std::string path) {
	std::deque<std::string> lines;

	std::string line;
	std::ifstream csv(path);

	if (!csv.is_open()) {
		throw std::runtime_error(std::format("Datafrane::load_csv: Could not open file {}", path));
	}

	while (getline(csv, line)) {
		lines.push_back(line);
	}
	csv.close();

	this->from_csv_format(lines);
}

std::vector<TypedRow> Dataframe::row_wise() {
	std::vector<TypedRow> output;
	std::vector<TypedCell> buffer;

	if (this->data.size() == 0) {
		return output;
	}

	size_t length = std::visit([](const auto& vec) -> size_t {
		return vec.size();
	}, this->data[0]);

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < this->data.size(); j++) {
			buffer.push_back(Utils::get_typed_cell_from_column(&(this->data[j]), i));
		}
		output.push_back(buffer);
		buffer.clear();
	}
	return output;
}

TypedColumn Dataframe::icol(size_t index) {
	return this->data[index];
}

TypedColumn Dataframe::col(std::string name) {
	auto index = std::find(this->column_names.begin(), this->column_names.end(), name);
	if (index == this->column_names.end()) {
		throw std::runtime_error("Dataframe::col: Provided column name does not exist");
	}

	return this->data[std::distance(this->column_names.begin(), index)];
}

TypedRow Dataframe::irow(size_t index) {
	TypedRow output;

	for (auto col : this->data) {
		output.push_back(Utils::get_typed_cell_from_column(&col, index));
	}

	return output;
}

Shape Dataframe::shape() {
	return {
		this->data.size(),
		Utils::column_length(&this->data[0])
	};
}

std::vector<std::string> Dataframe::names() {
	return this->column_names;
}

bool Dataframe::exists(std::string name) {
	return std::ranges::find(this->column_names, name) != this->column_names.end();
}

void Dataframe::add_column(std::string name, TypedColumn column) {
	if (this->exists(name)) {
		throw std::runtime_error(std::format("Dataframe::add_column: Column name '{}' already exists", name));
	}

	this->column_names.push_back(name);
	this->data.push_back(column);
}

void Dataframe::concat(Dataframe df) {
	for (auto name : df.names()) {
		if (this->exists(name)) continue;

		this->column_names.push_back(name);
		this->data.push_back(df.col(name));
	}
}

Dataframe Dataframe::take(std::initializer_list<std::string> col_names) {
	if (col_names.size() == 0) {
		throw std::runtime_error("Dataframe::move: Empty col_names provided.");
	}
	
	Dataframe target;
	for (int i = 0; i < col_names.size(); i++) {
		try {
			std::string name = col_names.begin()[i];
			target.add_column(name, this->col(col_names.begin()[i]));
		}
		catch (std::runtime_error err) {
			throw std::runtime_error(std::format("Dataframe::move: error when getting column contents ({})", err.what()));
		}
	}

	this->prune(col_names);
	return target;
}

void Dataframe::prune(std::initializer_list<std::string> col_names) {
	if (col_names.size() == 0) {
		throw std::runtime_error("Dataframe::prune: Empty col_names provided.");
	}

	this->data = std::views::zip(this->column_names, this->data) 
		| std::views::filter([&col_names](const std::tuple<std::string, TypedColumn>& col) {
			return (std::ranges::find(col_names, std::get<0>(col)) == col_names.end());
		}) 
		| std::views::transform([](const std::tuple<std::string, TypedColumn>& col) {
			return std::get<1>(col);
		})
		| std::ranges::to<std::vector>();

	std::erase_if(this->column_names, [&col_names](const std::string& name) {
		return std::ranges::find(col_names, name) != col_names.end();
		});
}

void Dataframe::iprune(std::initializer_list<int> col_index) {
	if (col_index.size() == 0) {
		throw std::runtime_error("Dataframe::prune: Empty col_names provided.");
	}

	for (int i = 0; i < col_index.size(); i++) {
		if (col_index.begin()[i] > this->data.size()) {
			continue;
		}

		this->data.erase(this->data.begin() + col_index.begin()[i]);
		this->column_names.erase(this->column_names.begin() + col_index.begin()[i]);
	}
}

void Dataframe::display() {
	std::vector<size_t> lengths;
	auto row_wise = this->row_wise();

	// set the max column sizes
	std::transform(this->column_names.begin(), this->column_names.end(), std::back_inserter(lengths),
		[](const std::string& s) {
			return s.length() + COLUMN_PADDING;
		});

	for (auto row : row_wise) {
		for (int i = 0; i < row.size(); i++) {
			if (Utils::to_string(row[i]).size() >= lengths[i]) {
				lengths[i] = Utils::to_string(row[i]).size() + COLUMN_PADDING;
			}
		}
	}
	// output data
	for (int i = 0; i < this->column_names.size(); i++) {
		std::cout << Utils::pad(this->column_names[i], lengths[i]);
	}

	std::string sep = std::string(std::accumulate(lengths.begin(), lengths.end(), 0), '-');
	std::cout << std::endl << sep << '\n';

	for (auto row : row_wise) {
		for (int i = 0; i < row.size(); i++) {
			std::cout << Utils::pad(Utils::to_string(row[i]), lengths[i]);
		}
		std::cout << std::endl;
	}
}