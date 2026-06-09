#include "mlp.h"
#include "./data/data.h"
#include "./data/preprocessing.h"

class MLP {
private:
	Dataframe data;
	Dataframe weights;
public:
	MLP() : data(Dataframe()) {};
	~MLP() {};

	void create(std::vector<int> hidden_layers);
};

void MLP::create(std::vector<int> hidden_layers){
	std::cout << "Loading" << std::endl;
	this->data.load_csv("./Iris.csv");
	std::cout << "Loaded" << std::endl;
	this->data.display();
	std::cout << "Pruning index column" << std::endl;
	this->data.prune({"Id"});
	std::cout << "Pruned index column" << std::endl;
	this->data.display();

	std::cout << "Loading" << std::endl;
	this->data.load_csv("./OHE_test.csv");
	std::cout << "Loaded" << std::endl;
	this->data.display();
	std::cout << "Performing OHE" << std::endl;
	Dataframe encoded = Preprocessing::one_hot_encoding(this->data.take({ "pet" }));

	this->data.concat(encoded);
	std::cout << "Performed OHE" << std::endl;
	std::cout << "Performing scaling" << std::endl;
	StandardScaler scaler;
	Dataframe scaled = scaler.fit_transform(this->data.take({ "age" }));

	this->data.concat(scaled);
	this->data.display();
}

int main()
{
	MLP mlp;

	try {
		mlp.create({3, 3});
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}
