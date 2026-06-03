#include "data.h"
#include "utils.h"

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

void Dataframe::from_csv_format(std::deque<std::string> lines, bool parse_column_names) {
	std::cout << "test1" << std::endl;
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

	std::cout << "building string cols" << std::endl;
	for (; !lines.empty(); lines.pop_front()) {
		std::vector<std::string> split = Utils::split(lines.front());
		if (split.size() != expected_length) {
			throw std::runtime_error("Dataframe::from_csv: Input row size error");
		}
		for (int i = 0; i < expected_length; i++) {
			columns[i].push_back(split[i]);
		}
	}
	std::cout << "built string cols" << std::endl;
	if (!parse_column_names) {
		for (int i = 0; i < columns[0].size(); i++) {
			column_names.push_back(std::format("col{}", i));
		}
	}

	this->columns = column_names;
	std::vector<TypedColumn> typed_columns;

	for (auto col : columns) {
		typed_columns.push_back(this->choose_column_type(col));
	}
	this->data = typed_columns;
	std::cout << "terst" << std::endl;
}

void Dataframe::display() {
	std::vector<int> lengths;

	auto row_wise = this->row_wise();

	// set the max column sizes
	std::transform(this->columns.begin(), this->columns.end(), std::back_inserter(lengths),
		[](const std::string& s) {
			return s.length() + 1;
		});
	for (auto row : row_wise) {
		for (int i = 0; i < row.size(); i++) {
			if (Utils::to_string(row[i]).size() >= lengths[i]) {
				lengths[i] = Utils::to_string(row[i]).size() + 1;
			}
		}
	}

	// output data
	for (int i = 0; i < this->columns.size(); i++) {
		std::cout << Utils::pad(this->columns[i], lengths[i]);
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

TypedColumn Dataframe::choose_column_type(std::vector<std::string> column) {
	return column;
}
TypedRows Dataframe::row_wise() {
	TypedRows output;
	std::vector<TypedCell> buffer;

	if (this->data.size() == 0) {
		return output;
	}

	size_t length = std::visit([](const auto& vec) -> size_t {
		return vec.size();
	}, this->data[0]);

	for (int i = 0; i < length; i++) {
		for (int j = 0; j < this->data.size(); j++) {
			buffer.push_back(Utils::get_typed_cell(&(this->data[j]), i));
		}
		output.push_back(buffer);
		buffer.clear();
	}

	return output;
}
float Dataframe::icol(size_t index) { return 1.0; }
float Dataframe::col(std::string name) { return 1.0; }
float Dataframe::irow(size_t index) { return 1.0; }