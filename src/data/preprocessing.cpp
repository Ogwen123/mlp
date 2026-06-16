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

			if constexpr (!(std::is_same_v<T, int> || std::is_same_v<T, std::string>)) {
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


void StandardScaler::fit(const Dataframe& data) {
	for (auto name : data.names()) {
		auto col_data = std::visit([name](const auto& vec) -> std::pair<double, double> {

			using T = std::decay_t<decltype(vec)>::value_type;

			if constexpr (!(std::is_same_v<T, int> || std::is_same_v<T, double>)) {
				throw std::runtime_error(std::format("Preprocessing::one_hot_encoding: Invalid column type provided ({})", typeid(T).name()));
			}
			else {
				double mean{ 0 };
				double std{ 0 };
				double m2{ 0 };
				int count{ 0 };
				std::cout << "starting " << name << std::endl;
				for (auto _item : vec) {
					double item = static_cast<double>(_item);

					count++;
					double old = mean;
					mean += (item - mean) / count;
					m2 += (item - old) * (item - mean);
				}
				std = sqrt(m2 / count);
				std::cout << "got std dev for " << name << std::endl;
				return std::pair<double, double>(mean, std);
			}
		}, data.col(name));

		this->data.insert({ name, {
			std::get<0>(col_data),
			std::get<1>(col_data)
		} });
	}
};

void StandardScaler::transform(Dataframe& data) {
	if (this->data.size() == 0) {
		throw std::runtime_error("StandardScaler::transform: No fit data");
	}

	Dataframe standard;

	for (auto name : data.names()) {
		auto iter = this->data.find(name);
		if (iter == this->data.end()) {
			throw std::runtime_error("StandardScaler::transform: Found column that was not provided in fit()");
		}
		PerColumnCharacteristics c = iter->second;

		auto new_col = std::visit([c](const auto& vec) -> std::vector<double> {
			std::vector<double> standardised;

			using T = std::decay_t<decltype(vec)>::value_type;

			if constexpr (!(std::is_same_v<T, int> || std::is_same_v<T, double>)) {
				throw std::runtime_error(std::format("Preprocessing::one_hot_encoding: Invalid column type provided ({})", typeid(T).name()));
			}
			else {
				for (auto item : vec) {
					standardised.push_back((item - c.mean) / c.std);
				}

				return standardised;
			}
		}, data.col(name));

		standard.add_column(name, new_col);
	}

	data = standard;
};

void StandardScaler::fit_transform(Dataframe& data) {
	std::cout << "hjuh" << std::endl;
	this->fit(data);
	std::cout << "huih" << std::endl;
	this->transform(data);
}