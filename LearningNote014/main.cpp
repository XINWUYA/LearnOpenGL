

#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "Text";
	CGraphicsApp App(1280, 720, Name);
	App.init();
	App.openDepthTest();
	//App.openBlending();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}