#pragma once

#include <vector>
#include <deque>
#include <string>
#include <format>

class Dataframe {
private:
	Dataframe(std::vector<std::string> columns, std::vector<std::vector<float>> data) : columns(columns), data(data) {}
	std::vector<std::string> columns;
	std::vector<std::vector<float>> data;

public:
	Dataframe() : columns(std::vector<std::string>()), data(std::vector<std::vector<float>>()) {};

	static Dataframe from_csv(std::deque<std::string> data, bool parse_column_names = true);
	float icol(size_t index);
	float col(std::string name);
	float irow(size_t index);
	void display();
};