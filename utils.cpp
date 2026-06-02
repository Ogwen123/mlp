#include "utils.h"

std::string Utils::pad(std::string input, int total_length) {
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

std::vector<std::string> Utils::split(std::string line, char delimiter) {
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

std::string Utils::to_string(TypedCell variant) {
	if (int const* pval = std::get_if<int>(&variant))
		return to_string(*pval);

	if (double const* pval = std::get_if<double>(&variant))
		return to_string(*pval);

	return std::get<std::string>(variant);
}

TypedCell Utils::get_typed_cell(TypedColumn* column, int index) {
	if (std::vector<int> const* pval = std::get_if<std::vector<int>>(column))
		return TypedCell((*pval)[index]);

	if (std::vector<double> const* pval = std::get_if<std::vector<double>>(column))
		return TypedCell((*pval)[index]);

	return (std::get<std::vector<std::string>>(*column))[index];
}
