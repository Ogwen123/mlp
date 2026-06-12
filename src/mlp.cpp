#include "mlp.h"


void MLP::create(std::vector<int> hidden_layers){
	
}

int main()
{
	// test dataframe
	Dataframe data;
	std::cout << "Loading" << std::endl;
	data.load_csv("./housing.csv");
	std::cout << "Loaded" << std::endl;
	std::cout << data.shape() << std::endl;

	{ // scope block so op is deleted
		Dataframe op = data.take({ "ocean_proximity" });
		op = Preprocessing::one_hot_encoding(op);
		data.concat(op);
	}
	data.display();

	MLP mlp;

	try {
		mlp.create({3, 3});
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}
