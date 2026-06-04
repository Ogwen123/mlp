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
    if (bool const* pval = std::get_if<bool>(&variant))
        return std::to_string(*pval);

    if (int const* pval = std::get_if<int>(&variant))
        return std::to_string(*pval);

    if (double const* pval = std::get_if<double>(&variant))
        return std::to_string(*pval);

    if (std::string const* pval = std::get_if<std::string>(&variant))
        return *pval;

    throw std::runtime_error("Utils::to_string: unhandled variant type");
}

TypedCell Utils::get_typed_cell_from_column(TypedColumn* column, int index) {
	if (std::vector<bool> const* pval = std::get_if<std::vector<bool>>(column))
		return TypedCell((*pval)[index]);

	if (std::vector<int> const* pval = std::get_if<std::vector<int>>(column))
		return TypedCell((*pval)[index]);

	if (std::vector<double> const* pval = std::get_if<std::vector<double>>(column))
		return TypedCell((*pval)[index]);

	return (std::get<std::vector<std::string>>(*column))[index];
}

template <typename TBool, typename TInt, typename TDouble, typename Variant>
std::string to_type_impl(Variant* v) {
	if (std::get_if<TBool>(v))   return "bool";
	if (std::get_if<TInt>(v))    return "int";
	if (std::get_if<TDouble>(v)) return "double";
	return "string";
}

std::string Utils::to_type(TypedCell* v) {
	return to_type_impl<bool, int, double>(v);
}

std::string Utils::to_type(TypedColumn* v) {
	return to_type_impl<std::vector<bool>, std::vector<int>, std::vector<double>>(v);
}