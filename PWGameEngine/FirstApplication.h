#pragma once

#include "PWGWindow.h"
#include "PWGDevice.h"
#include "PWGRenderer.h"
#include "PWGModel.h"
#include "PWGDescriptor.h"
#include "PWGGameObject.h"

#include <memory>
#include <vector>


/*
	Typically only 1 renderer per application
	Many render systems for materials etc
*/

namespace PWG {
	class FirstApplication
	{
	public:
		FirstApplication();
		~FirstApplication();

		FirstApplication(const FirstApplication&) = delete;
		FirstApplication& operator=(const FirstApplication&) = delete;
	public:
		static constexpr int WIDTH = 1280;
		static constexpr int HEIGHT = 720;

		void run();


	private:
		PWGWindow window{ WIDTH, HEIGHT, "PWGameEngine" };

		PWGDevice device{ window };
		PWGRenderer renderer{ window, device };

		std::unique_ptr<PWGDescriptorPool> globalPool{};

		PWGGameObject::Map gameObjects;

	private:
		void loadGameObjects();
	};

}