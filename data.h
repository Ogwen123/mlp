#pragma once

#include <iostream>
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
using TypedCell = std::variant<
	int,
	double,
	std::string
>;

using TypedRows = std::vector<std::vector<TypedCell>>;

/*
Store data per column, mimic a Pandas Dataframe
*/
class Dataframe {
private:
	Dataframe(std::vector<std::string> columns, std::vector<TypedColumn> data) : columns(columns), data(data) {}
	void from_csv_format(std::deque<std::string> data, bool parse_column_names = true);
	TypedColumn choose_column_type(std::vector<std::string> column);

	std::vector<std::string> columns;
	std::vector<TypedColumn> data;

public:
	Dataframe() : columns(std::vector<std::string>()), data(std::vector<TypedColumn>()) {};

	void load_csv(std::string path);
	TypedRows row_wise();
	float icol(size_t index);
	float col(std::string name);
	float irow(size_t index);
	void display();
};