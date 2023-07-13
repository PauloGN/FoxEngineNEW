#include <thread>
#include<FoxEngine/Inc/FoxEngine.h>

using namespace FoxEngine;
using namespace FoxEngine::Core;

int WINAPI WinMain(HINSTANCE instance, HINSTANCE, LPSTR, int)
{

	const std::string EngineName = "Fox Engine";

	LOG("Hello World, I am %s", EngineName.c_str());
	Sleep(1000);
	LOG("Hello World, I am %s", EngineName.c_str());

	const int n = 10;

	ASSERT(n == 2+5, "Assert, test break point");

	return 0;
}