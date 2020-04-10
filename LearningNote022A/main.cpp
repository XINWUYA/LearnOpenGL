

#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "AdvancedLighting_NormalMap";
	CGraphicsApp App(1280, 720, Name);
	App.init();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}