#include "GraphicsApp.h"
#include <iostream>
#include <string>

int main()
{
	std::string Name = "AdvancedOpenGL_UniformBufferObject";
	CGraphicsApp App(1280, 720, Name);
	App.init();
	App.openDepthTest();
	App.setCursorStatus(DISABLE);
	App.run();
	return 0;
}