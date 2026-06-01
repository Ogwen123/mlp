#include "mlp.h"
#include "data.h"
#include "reader.h"

class MLP {
private:
	Dataframe data;
public:
	MLP() {};
	~MLP() {};

	void create();
};

void MLP::create(){
	this->data = Reader::read_csv("./Iris.csv");
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
