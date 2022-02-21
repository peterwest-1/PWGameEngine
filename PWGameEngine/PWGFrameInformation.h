#pragma once

#include "PWGCamera.h"
#include "PWGGameObject.h"
#include <vulkan/vulkan.h>
namespace PWG {

#define MAX_LIGHTS 10


	struct PointLight
	{
		glm::vec4 position{};
		glm::vec4 color{};
	};

	struct GlobalUBO {
		glm::mat4 projection{ 1.f };
		glm::mat4 view{ 1.f };
		glm::vec4 ambientLightColor{ 1.f,1.f ,1.f ,.1f };
		PointLight pointLights[MAX_LIGHTS];
		int numLights;
	};

	struct PWGFrameInformation
	{
		int frameIndex;
		float frameTime;
		VkCommandBuffer commandBuffer;
		PWGCamera& camera;
		VkDescriptorSet globalDescriptorSet;
		PWGGameObject::Map& gameObjects;
	};

}

