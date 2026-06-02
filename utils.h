#pragma once

#include <iostream>
#include <fstream>
#include <deque>
#include <variant>
#include <vector>
#include <string>

#include "data.h"

namespace Utils {
	std::vector<std::string> split(std::string input, char delimiter = ',');
	std::string pad(std::string input, int total_length);
	std::string to_string(TypedCell variant);
	TypedCell get_typed_cell(TypedColumn* column, int index);
}