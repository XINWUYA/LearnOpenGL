#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "PointShadows";
	CGraphicsApp App(800, 600, Name);
	App.init();
	App.setCursorStatus(ECursorMode::DISABLE);
	App.run();
	return 0;
}