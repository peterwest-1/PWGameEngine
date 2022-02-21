#pragma once
#include "PWGWindow.h"
#include "PWGSwapChain.h"
#include "PWGDevice.h"
#include "PWGModel.h"
#include <memory>
#include <vector>
#include <cassert>
namespace PWG {
	class PWGRenderer
	{
	public:
		PWGRenderer(PWGWindow& window, PWGDevice& device);
		~PWGRenderer();

		PWGRenderer(const PWGRenderer&) = delete;
		PWGRenderer& operator=(const PWGRenderer&) = delete;

		bool isFrameInProgress() const { return isFrameStarted; };
		VkRenderPass getSwapChainRenderPass() const { return swapChain->getRenderPass(); };
		float getAspectRatio() const { return swapChain->extentAspectRatio(); };
		VkCommandBuffer getCurrentCommandBuffer() const {
			assert(isFrameStarted && "Cannot get command buffer when frame is not in progress");
			return commandBuffers[currentFrameIndex];
		}

		VkCommandBuffer beginFrame();
		void endFrame();
		void beginSwapChainRenderPass(VkCommandBuffer commandBuffer);
		void endSwapChainRenderPass(VkCommandBuffer commandBuffer);

		int getFrameIndex() const {
			assert(isFrameStarted && "Cannot get frame index while frame is not in progess");
			return currentFrameIndex;
		}
	private:
		PWGWindow& window;
		PWGDevice& device;
		std::unique_ptr<PWGSwapChain> swapChain;
		std::vector<VkCommandBuffer> commandBuffers;

		uint32_t currentImageIndex;
		int currentFrameIndex{ 0 };

	private:
		bool isFrameStarted{ false };

		void createCommandBuffers();
		void freeCommandBuffers();
		void recreateSwapChain();

	};

}

