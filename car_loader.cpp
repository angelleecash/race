#include "car_loader.h"

namespace race
{
	CarLoader::CarLoader(string carName):_carName(carName),_database(carName)
	{
		MyObject root = _database.GetMyObject("asset");

	}

	Car CarLoader::Load()
	{
		Car car;
		return car;
	}
}

