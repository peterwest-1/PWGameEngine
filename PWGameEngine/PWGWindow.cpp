#pragma warning(disable : 26812)

#include "PWGWindow.h"
#include <stdexcept>
namespace PWG {
	PWGWindow::PWGWindow(int width, int height, std::string name) : width{ width }, height{ height }, windowName{name}
	{
		initializeWindow();
	}

	PWGWindow::~PWGWindow()
	{
		glfwDestroyWindow(window);
		glfwTerminate();
	}

	void PWGWindow::createWindowSurface(VkInstance instance, VkSurfaceKHR* surface)
	{
		if (glfwCreateWindowSurface(instance, window, nullptr, surface) != VK_SUCCESS) {
			throw std::runtime_error("Failed to create window surface");
		}
	}

	void PWGWindow::frameBufferResizeCallback(GLFWwindow* window, int width, int height)
	{
		auto newWindow = reinterpret_cast<PWGWindow *>(glfwGetWindowUserPointer(window));
		newWindow->frameBufferResized = true;
		newWindow->width = width;
		newWindow->height = height;
	}

	void PWGWindow::initializeWindow() {
		glfwInit();
		glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
		glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);

		window = glfwCreateWindow(width, height, windowName.c_str(), nullptr, nullptr);
		glfwSetWindowUserPointer(window, this);
		glfwSetFramebufferSizeCallback(window, frameBufferResizeCallback);
	}
}
