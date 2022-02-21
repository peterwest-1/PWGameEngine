
#include "FirstApplication.h"
#include <cstdlib>
#include <iostream>

int main() {

	PWG::FirstApplication app{};

	try {
		app.run();
	}
	catch (const std::exception& e) {
		std::cerr << e.what() << std::endl;
		return EXIT_FAILURE;
	}
	return EXIT_SUCCESS;
}

/*

GRAPHICS PIPELINE

Vertex Index/Buffer
InputAssembly //FixedFunction
VertexShader - Programmable GLSL
Rasterization //FixedFunction
FragmentShader - Programmable GLSL
ColorBlending //FixedFunction
FrameBuffer

NOT SHOWING Tesselation or Geometry Shader Stages

*/