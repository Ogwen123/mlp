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
#include <list>

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

using TypedRow = std::vector<TypedCell>;

struct Shape {
	size_t rows;
	size_t columns;

	friend std::ostream& operator<<(std::ostream& os, const Shape& p) {
		os << "Shape(" << p.rows << " rows" << ", " << p.columns << " cols" << ")";
		return os;
	}
};

/*
Store data per column, mimic a Pandas Dataframe
*/
class Dataframe {
private:
	Dataframe(std::vector<std::string> columns, std::vector<TypedColumn> data) : column_names(columns), data(data) {}
	void from_csv_format(std::deque<std::string> data, bool parse_column_names = true);
	TypedColumn choose_column_type(const std::vector<std::string>& column);

	std::vector<std::string> column_names;
	std::vector<TypedColumn> data;

public:
	Dataframe() : column_names({}), data({}) {};
	void load_csv(std::string path);
	std::vector<TypedRow> row_wise();

	TypedColumn icol(size_t index);
	TypedColumn col(const std::string& name) const;
	TypedRow irow(size_t index);
	Shape shape();
	std::vector<std::string> names() const;
	// Check if a colum name already exists in the Dataframe
	bool exists(const std::string& name) const;

	// Throws an error if the column name already exists
	void add_column(const std::string& name, const TypedColumn& column);
	// Adds Dataframe data to this Dataframe, skips columns with the same name as an existing column
	void concat(const Dataframe& df);

	// Move specified rows into a new Dataframe and return it, the columns will be added to the new Dataframe in the order they are given in col_names
	Dataframe take(const std::initializer_list<std::string>& col_names);
	// Make a new dataframe with the rows from start (inclusive) to end (exclusive), if end is not supplied it will default to the end of the list. Copies without deleting from original Dataframe
	Dataframe copy_rows(int start, int end = -1);
	// Remove the specfied columns, ignore column names that don't exist
	void prune(const std::initializer_list<std::string>& col_names);
	// Remove specificed columns, ignores columns out of range
	void iprune(const std::initializer_list<int>& col_names);
	void display(int max_display = 150);
};