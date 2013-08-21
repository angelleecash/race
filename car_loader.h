#ifndef __CAR_LOADER_H__
#define __CAR_LOADER_H__

#include <string>
#include "car.h"
#include "database.h"

using std::string;


namespace race
{
	class CarLoader
	{
	public:
		CarLoader(string carName);
		Car Load();

	private:
		string _carName;
		Database _database;

	};
}
#endif