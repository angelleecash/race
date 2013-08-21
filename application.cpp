#include "application.h"
#include "car_loader.h"

namespace race
{


	Application& Application::GetInstance()
	{
		static Application app;
		return app;
	}

	Application::Application()
	{
	}

	void Application::Init()
	{
		CarLoader carLoader("./bugatti_veyron164supersport_2010.prefabs.sb");
		carLoader.Load();

	}


	void Application::Run()
	{

	}
}