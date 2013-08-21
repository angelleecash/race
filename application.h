#ifndef __APPLICATION_H__
#define __APPLICATION_H__

namespace race
{
	class Application
	{
	public:
		Application();

		static Application& GetInstance();
		void Init();
		void Run();

	private:

	};

}
#endif