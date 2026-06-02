#pragma once

#include "data.h"

namespace Preprocessing {
	Dataframe one_hot_encoding(Dataframe data);
	Dataframe normalise(Dataframe data);
}