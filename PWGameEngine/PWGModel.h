#pragma once
#include "PWGDevice.h"
#include "PWGBuffer.h"
#include <vector>

#define GLM_FORCE_RADIANS
#define GLM_FORCE_DEPTH_ZERO_TO_ONE
#include <glm/glm.hpp>

#include <memory>
namespace PWG {
	class PWGModel
	{
	public:
		struct Vertex {
			glm::vec3 position{};
			glm::vec3 color{};
			glm::vec3 normal{};
			glm::vec2 uv{};

			static std::vector<VkVertexInputBindingDescription> getBindingDescriptions();
			static std::vector<VkVertexInputAttributeDescription> getAttributeDescriptions();

			bool operator==(const Vertex& other) const {
				return position == other.position
					&& color == other.color
					&& normal == other.normal
					&& uv == other.uv;

			}
		};

		struct Builder
		{
			std::vector<Vertex> vertices{};
			std::vector<uint32_t> indices{};

			void loadModel(const std::string& filepath);
		};

		PWGModel(PWGDevice& device, const PWGModel::Builder& builder);
		~PWGModel();

		PWGModel(const PWGModel&) = delete;
		PWGModel& operator=(const PWGModel&) = delete;

		static std::unique_ptr<PWGModel> createModelFromFile(PWGDevice& device, const std::string& filepath);

		void bind(VkCommandBuffer commandBuffer);
		void draw(VkCommandBuffer commandBuffer);

	private:
		PWGDevice& device;

		std::unique_ptr<PWGBuffer> vertexBuffer;
		uint32_t vertexCount;

		void createVertexBuffers(const std::vector<Vertex>& vertices);

		std::unique_ptr<PWGBuffer> indexBuffer;
		uint32_t indexCount;

		void createIndexBuffers(const std::vector<uint32_t>& indices);

		bool hasIndexBuffer = false;
	};
}
