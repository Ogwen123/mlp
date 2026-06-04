#pragma once

#include <iostream>
#include <vector>
#include <variant>
#include <deque>
#include <string>
#include <format>
#include <algorithm>
#include <iterator>
#include <numeric>
#include <ranges>

using TypedColumn = std::variant<
	std::vector<bool>,
	std::vector<int>,
	std::vector<double>,
	std::vector<std::string>
>;
using TypedCell = std::variant<
	bool,
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
	Dataframe(std::vector<std::string> columns, std::vector<TypedColumn> data) : column_names(columns), data(data) {}
	void from_csv_format(std::deque<std::string> data, bool parse_column_names = true);
	TypedColumn choose_column_type(std::vector<std::string> column);

	std::vector<std::string> column_names;
	std::vector<TypedColumn> data;

public:
	Dataframe() : column_names(std::vector<std::string>()), data(std::vector<TypedColumn>()) {};

	void load_csv(std::string path);
	TypedRows row_wise();
	float icol(size_t index);
	float col(std::string name);
	float irow(size_t index);
	// Move specified rows into a new Dataframe and return it
	Dataframe move(std::string col_names[]);
	// Remove the specfied columns
	void prune(std::string col_names[]);
	void display();
};