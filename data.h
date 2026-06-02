#pragma once

#include <vector>
#include <variant>
#include <deque>
#include <string>
#include <format>
#include <algorithm>
#include <iterator>

using TypedColumn = std::variant<
	std::vector<int>,
	std::vector<double>,
	std::vector<std::string>
>;

class Dataframe {
private:
	Dataframe(std::vector<std::string> columns, std::vector<TypedColumn> data) : columns(columns), data(data) {}
	std::vector<std::string> columns;
	std::vector<TypedColumn> data;

public:
	Dataframe() : columns(std::vector<std::string>()), data(std::vector<std::vector<double>>()) {};

	static Dataframe from_csv(std::deque<std::string> data, bool parse_column_names = true);
	float icol(size_t index);
	float col(std::string name);
	float irow(size_t index);
	void display();
};