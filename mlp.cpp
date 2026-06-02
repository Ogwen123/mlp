#include "mlp.h"
#include "data.h"

class MLP {
private:
	Dataframe data;
	Dataframe weights;
public:
	MLP() : data(Dataframe()) {};
	~MLP() {};

	void create();
};

void MLP::create(){
	this->data.load_csv("./Iris.csv");
	data.display();
}

int main()
{
	MLP mlp;

	try {
		mlp.create();
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}
