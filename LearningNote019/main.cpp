#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "Blinn-Phong";
	CGraphicsApp App(800, 600, Name);
	App.init();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}