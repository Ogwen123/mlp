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
	return std::visit([](const auto& item) -> std::string {
		using T = typename std::decay_t<decltype(item)>;
		
		if constexpr (std::is_same_v<T, std::string>) {
			return item;
		}
		else {
			return std::to_string(item);
		}
	}, variant);
}

TypedCell Utils::get_typed_cell_from_column(TypedColumn* column, size_t index) {
	return std::visit([index](const auto& vec) -> TypedCell {
		return TypedCell(vec[index]);
	}, *column);
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

TypedColumn Utils::take_range(TypedColumn* column, int start, int end) {
	return std::visit([start, end](const auto& vec) -> TypedColumn {
		using T = typename std::decay_t<decltype(vec)>::value_type;

		return std::vector<T>(vec.begin() + start, vec.begin() + end);
	}, *column);
}

size_t Utils::column_length(TypedColumn* col) {
	return std::visit([](const auto& vec) -> std::size_t {
		return vec.size();
	}, *col); 
}