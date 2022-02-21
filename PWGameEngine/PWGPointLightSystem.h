#pragma once

#include "PWGPipeline.h"
#include "PWGDevice.h"
#include "PWGModel.h"
#include "PWGGameObject.h"
#include "PWGFrameInformation.h"
#include "PWGCamera.h"

#include <memory>
#include <vector>

namespace PWG {
	class PWGPointLightSystem
	{
	public:
		PWGPointLightSystem(PWGDevice& device, VkRenderPass renderPass, VkDescriptorSetLayout globalSetLayout);
		~PWGPointLightSystem();

		PWGPointLightSystem(const PWGPointLightSystem&) = delete;
		PWGPointLightSystem& operator=(const PWGPointLightSystem&) = delete;
	public:
		void update(PWGFrameInformation& frameInfo, GlobalUBO& ubo);
		void render(PWGFrameInformation& frameInfo);

	private:

		PWGDevice& device;
		std::unique_ptr<PWGPipeline> pipeline;
		VkPipelineLayout pipelineLayout;

	private:

		void createPipelineLayout(VkDescriptorSetLayout globalSetLayout);
		void createPipeline(VkRenderPass renderPass);

	};

}