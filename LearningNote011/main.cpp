#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "Text";
	CGraphicsApp App(1280, 720, Name);
	App.init();
	App.importModel("../ModelSources/nanosuit/nanosuit.obj");
	App.openDepthTest();
	App.openStencilTest();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}