#pragma once

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <string>

namespace PWG {
	class PWGWindow
	{
		GLFWwindow* window;

		void initializeWindow();
		int width;
		int height;
		bool frameBufferResized = false;

		std::string windowName;
	public:
		PWGWindow(int width, int height, std::string name);
		~PWGWindow();

		PWGWindow(const PWGWindow&) = delete;
		PWGWindow& operator=(const PWGWindow&) = delete;

		VkExtent2D getExtent() { return { static_cast<uint32_t>(width),static_cast<uint32_t>(height) }; }

		bool shouldClose() { return glfwWindowShouldClose(window); }

		bool wasWindowResized() { return frameBufferResized; }
		void resetWindowResizedFlag() { frameBufferResized = false; }
		void createWindowSurface(VkInstance instance, VkSurfaceKHR* surface);

		GLFWwindow* getGLFWWindow() const { return window; };

	private:
		static void frameBufferResizeCallback(GLFWwindow* window, int width, int height);
	};
}