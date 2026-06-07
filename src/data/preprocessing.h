#pragma once

#include <set>

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
	std::vector<PerColumnCharacteristics> data;
public:
	StandardScaler() : data({}) {};
	// calculates and stores the mean and standard deviation of each column of data, all columns must be numeric (int or float)
	void fit(Dataframe data);
	// applies z-score standarisation to data using calculated mean and standard deviation for each column, transform input must be the same shape as fit data.
	Dataframe transform(Dataframe data);
};