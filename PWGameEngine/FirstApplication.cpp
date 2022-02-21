#pragma warning(disable : 26812)

#include "FirstApplication.h"

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>
#include <glm/gtc/constants.hpp>

#include "PWGCamera.h"
#include "PWGBuffer.h"
#include "PWGSimpleRenderSystem.h"
#include "PWGPointLightSystem.h"
#include "PWGUserInputController.h"

#include <stdexcept>
#include <array>
#include <vector>
#include <iostream>
#include <chrono>

namespace PWG {



	FirstApplication::FirstApplication()
	{
		globalPool =
			PWGDescriptorPool::Builder(device)
			.setMaxSets(PWGSwapChain::MAX_FRAMES_IN_FLIGHT)
			.addPoolSize(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, PWGSwapChain::MAX_FRAMES_IN_FLIGHT)
			.build();
		loadGameObjects();
	}

	FirstApplication::~FirstApplication()
	{

	}

	void PWG::FirstApplication::run()
	{

		std::vector<std::unique_ptr<PWGBuffer>> globalUBOBuffers(PWGSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalUBOBuffers.size(); i++) {
			globalUBOBuffers[i] = std::make_unique<PWGBuffer>(
				device,
				sizeof(GlobalUBO),
				1,
				VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
				VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT);
			globalUBOBuffers[i]->map();
		}

		auto globalSetLayout = PWGDescriptorSetLayout::Builder(device).addBinding(0, VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_ALL_GRAPHICS).build();

		std::vector<VkDescriptorSet> globalDescriptorSets(PWGSwapChain::MAX_FRAMES_IN_FLIGHT);
		for (int i = 0; i < globalDescriptorSets.size(); i++)
		{
			auto bufferInfo = globalUBOBuffers[i]->descriptorInfo();
			PWGDescriptorWriter(*globalSetLayout, *globalPool)
				.writeBuffer(0, &bufferInfo)
				.build(globalDescriptorSets[i]);
		}

		PWGSimpleRenderSystem simpleRenderSystem{ device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PWGPointLightSystem pointLightSystem{ device, renderer.getSwapChainRenderPass(), globalSetLayout->getDescriptorSetLayout() };
		PWGCamera camera{};

		auto viewerObject = PWGGameObject::createGameObject();
		viewerObject.transform.translation.z = -5.5f;
		PWGUserInputController cameraController{};

		auto currentTime = std::chrono::high_resolution_clock::now();

		while (!window.shouldClose()) {

			glfwPollEvents(); //May block

			auto newTime = std::chrono::high_resolution_clock::now();
			float frameTime = std::chrono::duration<float, std::chrono::seconds::period>(newTime - currentTime).count();
			currentTime = newTime;

			cameraController.moveInPlaneXZ(window.getGLFWWindow(), frameTime, viewerObject);
			camera.setViewYXZ(viewerObject.transform.translation, viewerObject.transform.rotation);

			float aspect = renderer.getAspectRatio();
			camera.setPerspectiveProjection(glm::radians(50.f), aspect, 0.1f, 10.f);

			if (auto commandBuffer = renderer.beginFrame())
			{
				int frameIndex = renderer.getFrameIndex();
				PWGFrameInformation frameInfo{ frameIndex, frameTime, commandBuffer, camera, globalDescriptorSets[frameIndex], gameObjects };
				//UPDATE OBJECTS 
				GlobalUBO ubo{};

				ubo.projection = camera.getProjection();
				ubo.view = camera.getView();

				pointLightSystem.update(frameInfo, ubo);

				globalUBOBuffers[frameIndex]->writeToBuffer(&ubo);
				globalUBOBuffers[frameIndex]->flush();


				//RENDER
				renderer.beginSwapChainRenderPass(commandBuffer);
				simpleRenderSystem.renderGameObjects(frameInfo);
				pointLightSystem.render(frameInfo);
				renderer.endSwapChainRenderPass(commandBuffer);
				renderer.endFrame();
			}
		}

		vkDeviceWaitIdle(device.device());
	}


	void FirstApplication::loadGameObjects()
	{

		std::shared_ptr<PWGModel> vaseModel = PWGModel::createModelFromFile(device, "flat_vase.obj");
		auto flatVaseObject = PWGGameObject::createGameObject();
		flatVaseObject.model = vaseModel;
		flatVaseObject.transform.translation = { -0.5f, 0.5f, 0.f };
		flatVaseObject.transform.scale = glm::vec3(3.f);
		gameObjects.emplace(flatVaseObject.getID(), std::move(flatVaseObject));

		std::shared_ptr<PWGModel> smoothVaseModel = PWGModel::createModelFromFile(device, "smooth_vase.obj");
		auto smoothVaseObject = PWGGameObject::createGameObject();
		smoothVaseObject.model = smoothVaseModel;
		smoothVaseObject.transform.translation = { 0.5f, 0.5f, 0.f };
		smoothVaseObject.transform.scale = glm::vec3(3.f);

		gameObjects.emplace(smoothVaseObject.getID(), std::move(smoothVaseObject));

		std::shared_ptr<PWGModel> quadModel = PWGModel::createModelFromFile(device, "quad.obj");
		auto quadObject = PWGGameObject::createGameObject();
		quadObject.model = quadModel;
		quadObject.transform.translation = { 0.f, 0.5f, 0.f };
		quadObject.transform.scale = glm::vec3(3.f);

		gameObjects.emplace(quadObject.getID(), std::move(quadObject));

		std::vector<glm::vec3> lightColors{
			{1.f, .1f, .1f},
			{.1f, .1f, 1.f},
			{.1f, 1.f, .1f},
			{1.f, 1.f, .1f},
			{.1f, 1.f, 1.f},
			{1.f, 1.f, 1.f},
			{.1f, 1.f, .1f},
			{1.f, 1.f, .1f},
			{.1f, 1.f, 1.f},
			{1.f, 1.f, 1.f}   //
		};

		for (int i = 0; i < lightColors.size(); i++) {
			auto pointLight = PWGGameObject::makePointLight(0.5f);
			pointLight.color = lightColors[i];
			auto rotateLight = glm::rotate(
				glm::mat4(1.f),
				(i * glm::two_pi<float>()) / lightColors.size(),
				{ 0.f, -1.f, 0.f });
			pointLight.transform.translation = glm::vec3(rotateLight * glm::vec4(-1.f, -1.f, -1.f, 1.f));
			gameObjects.emplace(pointLight.getID(), std::move(pointLight));
		}

	}

}
