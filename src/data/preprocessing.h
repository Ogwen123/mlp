#pragma once

#include <set>
#include <unordered_map>

#include "data.h"

namespace Preprocessing {
	// Perform one-hot encoding, assumes all given columns are categorical, accepts string and int columns
	Dataframe one_hot_encoding(Dataframe data);

}

struct PerColumnCharacteristics {
	double mean;
	double std;
};

// Applies z-score standardisation to a Dataframe of numeric data
class StandardScaler {
private:
	std::unordered_map<std::string, PerColumnCharacteristics> data;
public:
	// Calculates and stores the mean and standard deviation of each column of data, all columns must be numeric (int or float). Data is associated with the column name.
	void fit(const Dataframe& data);
	// Applies z-score standarisation to data using calculated mean and standard deviation for each column, transform input must be the same shape as fit data and have the same column names.
	void transform(Dataframe& data);
	// Apply the fit and transform functions in one step.
	void fit_transform(Dataframe& data);
};

class OneHotEncoder {
private:
	std::unordered_map<std::string, std::vector<std::string>> data;
public:
	// Computes and stored each category for each column
	void fit(const Dataframe& data);
	// Applies one hot encoding using the fitted categories for each column.
	void transform(Dataframe& data);
	// Apply the fit and transform functions in one step.
	void fit_transform(Dataframe& data);
};
