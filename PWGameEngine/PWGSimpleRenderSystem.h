#pragma once

#include "PWGPipeline.h"
#include "PWGDevice.h"
#include "PWGModel.h"
#include "PWGGameObject.h"
#include "PWGFrameInformation.h"
#include <memory>
#include <vector>
#include "PWGCamera.h"
namespace PWG {
	class PWGSimpleRenderSystem
	{
	public:
		PWGSimpleRenderSystem(PWGDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PWGSimpleRenderSystem();

		PWGSimpleRenderSystem(const PWGSimpleRenderSystem&) = delete;
		PWGSimpleRenderSystem& operator=(const PWGSimpleRenderSystem&) = delete;
	public:
		void renderGameObjects(PWGFrameInformation& frameInfo);

	private:

		PWGDevice& device;
		std::unique_ptr<PWGPipeline> pipeline;
		VkPipelineLayout pipelineLayout;

	private:

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

	};

}