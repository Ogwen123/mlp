#include "mlp.h"


void MLP::create(std::vector<int> hidden_layers){
	
}

int main()
{
	// test dataframe
	Dataframe data;
	std::cout << "Loading" << std::endl;
	data.load_csv("./Iris.csv");
	std::cout << "Loaded" << std::endl;
	data.display();
	std::cout << "Pruning index column" << std::endl;
	data.prune({ "Id" });
	std::cout << "Pruned index column" << std::endl;
	data.display();

	std::cout << "Loading" << std::endl;
	data.load_csv("./OHE_test.csv");
	std::cout << "Loaded" << std::endl;
	data.display();
	std::cout << "Performing OHE" << std::endl;
	Dataframe encoded = Preprocessing::one_hot_encoding(data.take({ "pet" }));

	data.concat(encoded);
	std::cout << "Performed OHE" << std::endl;
	std::cout << "Performing scaling" << std::endl;
	StandardScaler scaler;
	Dataframe scaled = scaler.fit_transform(data.take({ "age" }));

	data.concat(scaled);
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
