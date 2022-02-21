#pragma once
#include <string>
#include <vector>
#include "PWGDevice.h"


namespace PWG {

	struct PWGPipelineConfigInfo
	{
		PWGPipelineConfigInfo() = default;
		PWGPipelineConfigInfo(const PWGPipelineConfigInfo&) = delete;
		PWGPipelineConfigInfo& operator=(const PWGPipelineConfigInfo&) = delete;

		std::vector<VkVertexInputBindingDescription> bindingDescriptions{};
		std::vector<VkVertexInputAttributeDescription> attributeDescriptions{};
		VkPipelineViewportStateCreateInfo viewportInfo;
		VkPipelineInputAssemblyStateCreateInfo inputAssemblyInfo;
		VkPipelineRasterizationStateCreateInfo rasterizationInfo;
		VkPipelineMultisampleStateCreateInfo multisampleInfo;
		VkPipelineColorBlendAttachmentState colorBlendAttachment;
		VkPipelineColorBlendStateCreateInfo colorBlendInfo;
		VkPipelineDepthStencilStateCreateInfo depthStencilInfo;
		std::vector<VkDynamicState> dynamicStateEnables;
		VkPipelineDynamicStateCreateInfo dynamicStateInfo;
		VkPipelineLayout pipelineLayout = nullptr;
		VkRenderPass renderPass = nullptr;
		uint32_t subpass = 0;
	};

	class PWGPipeline
	{
	public:
		PWGPipeline(PWGDevice& device, const std::string& vertFilepath, const std::string& fragFilepath, const PWGPipelineConfigInfo& pipelineConfigInfo);
		~PWGPipeline();

		PWGPipeline(const PWGPipeline&) = delete;
		PWGPipeline& operator=(const PWGPipeline&) = delete;

		static void defaultPipelineConfigInfo(PWGPipelineConfigInfo& configInfo);

		void bind(VkCommandBuffer commandBuffer);
	private:
		static std::vector<char> readFile(const std::string& filepath);
		void createGraphicsPipeline(const std::string& vertFilepath, const std::string& fragFilepath, const PWGPipelineConfigInfo& pipelineConfigInfo);

		void createShaderModule(const std::vector<char>& code, VkShaderModule* shaderModule);

		PWGDevice& device;

		VkPipeline graphicsPipeline;
		VkShaderModule vertShaderModule;
		VkShaderModule fragShaderModule;

	};
}

