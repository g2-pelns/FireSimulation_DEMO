//Filename : main.cpp
#include "SystemClass.h"
#include "WinDef.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR pScmdline, int iCmdshow)
{
	SystemClass* System;
	bool result;

	//Create the system object
	System = new SystemClass;
	if (!System)
	{
		return 0;
	}

	//Initialize and run the system object
	result = System->Init();
	if (result)
	{
		System->Run();
	}

	//Shutdown and release the system object
	System->ShutDown();
	delete System;
	System = 0;

	return 0;
}