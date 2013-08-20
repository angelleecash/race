#include "car_loader.h"

CarLoader::CarLoader(string carName):_carName(carName),_database(carName)
{
}

Car CarLoader::Load()
{
	Car car;
	return car;
}

