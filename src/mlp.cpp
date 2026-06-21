#include "mlp.h"

constexpr double TRAINING_PERCENT = 0.7;
constexpr double VALIDATION_PERCENT = 0.15;
constexpr double TESTING_PERCENT = 0.15;

void MLP::create(std::vector<int> hidden_layers){
	
}

int main()
{
	try {
		// test dataframe
		auto data = std::make_unique<Dataframe>();
		std::cout << "Loading" << std::endl;
		data->load_csv("./housing.csv");
		std::cout << "Loaded" << std::endl;
		std::cout << data->shape() << std::endl;

		{ // scope block so op is deleted
			Dataframe op = data->take({ "ocean_proximity" });
			op = Preprocessing::one_hot_encoding(op);
			data->concat(op);
		}
		data->display();
		data->purge_NaNs();

		auto rows = data->shape().rows;

		int training_end = (int) ((int)rows * TRAINING_PERCENT);
		int validation_end = (int) ((int)rows * VALIDATION_PERCENT + training_end);

		Dataframe training_x = data->copy_rows(0, training_end);
		Dataframe training_y = training_x.take({ "median_house_value" });

		Dataframe validation_x = data->copy_rows(training_end, validation_end);
		Dataframe validation_y = validation_x.take({ "median_house_value" });

		Dataframe testing_x = data->copy_rows(validation_end);
		Dataframe testing_y = testing_x.take({ "median_house_value" });

		data.reset();

		std::initializer_list<std::string> numeric = { "longitude","latitude","housing_median_age","total_rooms","total_bedrooms","population","households","median_income" };

		auto training_x_numeric = training_x.take(numeric);
		auto validation_x_numeric = validation_x.take(numeric);
		auto testing_x_numeric = testing_x.take(numeric);

		StandardScaler scaler;
		scaler.fit_transform(training_x_numeric);
		scaler.transform(validation_x_numeric);
		scaler.transform(testing_x_numeric);
		validation_x_numeric.display();
		MLP mlp;

		mlp.create({3, 3});
	}
	catch (const std::exception& err) {
		std::cout << err.what() << std::endl;
	}
	return 0;
}
