#pragma once

#include <iostream>
#include <string>

#include "./data/data.h"
#include "./data/preprocessing.h"

class MLP {
private:
	Dataframe weights;
public:
	void create(std::vector<int> hidden_layers);
	void input(Dataframe data);
	void input(std::string path);
};