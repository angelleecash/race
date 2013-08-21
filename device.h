#ifndef __DEVICE_H__
#define __DEVICE_H__

#include <string>

using std::string;

namespace race
{

	typedef enum 
	{
		iPhone4,
		iPhone5,
		iPad2,
		iPad3,
		Windows,
		Mac,
		SamsungI9100
	} Device;

	class DeviceInfo
	{
	public:
		DeviceInfo(string name, int width, int height)
			:_name(name),
			_width(width),
			_height(height)
		{}


		string& GetName(){return _name;}
		int GetWidth(){return _width;}
		int GetHeight(){return _height;}

	private:
		string _name;
		int _width, _height;
	};



	DeviceInfo GetDeviceInfo()
	{
		static DeviceInfo deviceInfo("windows", 1024, 768);
		return deviceInfo;
	}

}

#endif