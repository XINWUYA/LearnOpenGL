#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "Anti Aliasing";
	CGraphicsApp App(800, 600, Name);
	App.init();
	App.openDepthTest();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}