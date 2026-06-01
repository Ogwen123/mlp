#include "reader.h"

Dataframe Reader::read_csv(std::string path) {
	std::deque<std::string> lines;

	std::string line;
	std::ifstream csv(path);

	if (!csv.is_open()) {
		throw std::runtime_error(std::format("Reader::read_csv: Could not open file {}", path));
	}

	while (getline(csv, line)) {
		lines.push_back(line);
	}
	csv.close();

	Dataframe data = Dataframe::from_csv(lines);

	return data;
}

std::vector<std::string> Reader::scsv(std::string line) {
	return Reader::split(line, ',');
}

std::vector<float> Reader::fcsv(std::string line) {
	std::vector<std::string> split = Reader::split(line, ',');
	std::vector<float> res;

	for (auto item : split) {
		res.push_back(atof(item.c_str()));
	}

	return res;
}

std::vector<std::string> Reader::split(std::string line, char delimiter) {
	std::vector<std::string> split;
	std::string buffer;

	for (auto s : line) {
		if (s == delimiter) {
			split.push_back(buffer);
			buffer = std::string();
		}
		else {
			buffer += s;
		}
	}
	if (buffer.size() > 0) {
		split.push_back(buffer);
	}
	return split;
}