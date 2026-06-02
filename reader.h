#pragma once

#include "data.h"
#include <iostream>
#include <fstream>
#include <deque>

namespace Reader {
	Dataframe read_csv(std::string path);
	std::vector<std::string> scsv(std::string line);
	std::vector<double> dcsv(std::string line);
	std::vector<std::string> split(std::string input, char delimiter);
}