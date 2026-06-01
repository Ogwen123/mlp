#include "data.h"
#include "reader.h"

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
		expected_length = Reader::fcsv(lines.front()).size();
	}

	for (; !lines.empty(); lines.pop_front()) {
		std::vector<float> parsed = Reader::fcsv(lines.front());
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
	for (auto col : this->columns) {
		std::cout << col;
	}
	std::cout << std::endl;
	for (auto row : data) {
		for (auto item : row) {
			std::cout << item << " ";
		}
		std::cout << std::endl;
	}
}
float Dataframe::icol(size_t index) { return 1.0; }
float Dataframe::col(std::string name) { return 1.0; }
float Dataframe::irow(size_t index) { return 1.0; }