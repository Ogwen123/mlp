#pragma once

#include <iostream>
#include <fstream>
#include <deque>
#include <variant>
#include <vector>
#include <string>

#include "./data/data.h"

namespace Utils {
	std::vector<std::string> split(std::string input, char delimiter = ',');
	std::string pad(std::string input, int total_length);
	std::string to_string(TypedCell variant);
	std::string to_type(TypedCell* v);
	std::string to_type(TypedColumn* v);
	TypedCell get_typed_cell_from_column(TypedColumn* column, size_t index);
	// Returns values from start (inclusive) to end (exclusive)
	TypedColumn take_range(TypedColumn* column, int start, int end);
	size_t column_length(TypedColumn* column);
}