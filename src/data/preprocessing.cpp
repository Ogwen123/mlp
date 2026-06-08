#include "preprocessing.h"

auto str = [](const auto& val) -> std::string {
	using V = std::decay_t<decltype(val)>;
	if constexpr (std::is_same_v<V, std::string>)
		return val;
	else
		return std::to_string(val);
	};

Dataframe Preprocessing::one_hot_encoding(Dataframe data) {
	Dataframe encoded;

	for (auto name : data.names()) {
		auto col = std::visit([name](const auto& vec) -> Dataframe {
			Dataframe expanded;

			using T = std::decay_t<decltype(vec)>::value_type;
			std::cout << typeid(T).name() << std::endl;

			if (!(std::is_same_v<T, int> || std::is_same_v<T, std::string>)) {
				throw std::runtime_error(std::format("Preprocessing::one_hot_encoding: Invalid column type provided ({})", typeid(T).name()));
			}
			else {
				std::set<std::string> names;

				for (int i = 0; i < vec.size(); i++) {
					names.insert(str(vec[i]));
				}

				for (auto var_name : names) {
					std::vector<int> col;
					for (int i = 0; i < vec.size(); i++) {
						if (var_name == str(vec[i])) {
							col.push_back(1);
						}
						else {
							col.push_back(0);
						}
					}
					expanded.add_column(name + "_" + var_name, TypedColumn(col));
				}
				return expanded;
			}
		}, data.col(name));
		encoded.concat(col);
	}

	return encoded;
}

void StandardScaler::fit(Dataframe data) {
	for (auto name : data.names()) {
		auto col_data = std::visit([name](const auto& vec) -> std::pair<double, double> {
			Dataframe expanded;

			using T = std::decay_t<decltype(vec)>::value_type;
			std::cout << typeid(T).name() << std::endl;

			if constexpr (!(std::is_same_v<T, int> || std::is_same_v<T, double>)) {
				throw std::runtime_error(std::format("Preprocessing::one_hot_encoding: Invalid column type provided ({})", typeid(T).name()));
			}
			else {
				double mean = 0;
				double std = 0;
				double m2 = 0;
				int count = 0;

				for (auto _item : vec) {
					double item = static_cast<double>(_item);

					count++;
					double old = mean;
					mean += (item - mean) / count;
					m2 += (item - old) * (item - mean);
				}
				std = sqrt(m2 / count);

				return std::pair<double, double>(mean, std);
			}
			}, data.col(name));

		this->data.push_back({
			std::get<0>(col_data),
			std::get<1>(col_data)
		});
	}
};
Dataframe StandardScaler::transform(Dataframe data) { return data; };