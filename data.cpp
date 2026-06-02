#include "data.h"
#include "reader.h"

std::string pad(std::string input, int total_length) {
	std::string output;

	for (int i = 0; i < total_length; i++) {
		if (i < input.size()) {
			output += input[i];
		}
		else {
			output += ' ';
		}
	}

	return output;
}

Dataframe Dataframe::from_csv(std::deque<std::string> lines, bool parse_column_names) {
	if (lines.size() == 0) {
		std::cout << "no data" << std::endl;

		throw std::runtime_error("Dataframe::from_csv: Empty data");
	}

	std::vector<std::string> columns;
	std::vector<std::vector<float>> data;

	size_t expected_length = 0;

	if (parse_column_names) {
		columns = Reader::scsv(lines.front());
		lines.pop_front();

		expected_length = columns.size();
	}
	else {
		expected_length = Reader::dcsv(lines.front()).size();
	}

	for (; !lines.empty(); lines.pop_front()) {
		std::vector<float> parsed = Reader::dcsv(lines.front());
		if (parsed.size() != expected_length) {
			throw std::runtime_error("Dataframe::from_csv: Input row size error");
		}
		data.push_back(parsed);
	}

	if (!parse_column_names) {
		for (int i = 0; i < data[0].size(); i++) {
			columns.push_back(std::format("col{}", i));
		}
	}

	return Dataframe::Dataframe(columns, data);
}

void Dataframe::display() {
	std::vector<int> lengths;

	// set the max column sizes
	std::transform(this->columns.begin(), this->columns.end(), std::back_inserter(lengths),
		[](const std::string& s) {
			return s.length() + 1;
		});
	for (auto row : data) {
		for (int i = 0; i < row.size(); i++) {
			if (std::to_string(row[i]).size() > lengths[i]) {
				lengths[i] = std::to_string(row[i]).size() + 1;
			}
		}
	}

	// output data
	for (int i = 0; i < this->columns.size(); i++) {
		std::cout << pad(this->columns[i], lengths[i]);

	}
	std::cout << std::endl;

	for (auto row : data) {
		for (int i = 0; i < row.size(); i++) {
			std::cout << pad(std::to_string(row[i]), lengths[i]);
		}
		std::cout << std::endl;
	}
}

float Dataframe::icol(size_t index) { return 1.0; }
float Dataframe::col(std::string name) { return 1.0; }
float Dataframe::irow(size_t index) { return 1.0; }