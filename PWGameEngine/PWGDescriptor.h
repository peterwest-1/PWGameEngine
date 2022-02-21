#pragma once
#include "PWGDevice.h"

#include <memory>
#include <unordered_map>
#include <vector>

namespace PWG {

	class PWGDescriptorSetLayout {
	public:
		class Builder {
		public:
			Builder(PWGDevice& device) : device{ device } {}

			Builder& addBinding(
				uint32_t binding,
				VkDescriptorType descriptorType,
				VkShaderStageFlags stageFlags,
				uint32_t count = 1);
			std::unique_ptr<PWGDescriptorSetLayout> build() const;

		private:
			PWGDevice& device;
			std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings{};
		};

		PWGDescriptorSetLayout(
			PWGDevice& device, std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings);
		~PWGDescriptorSetLayout();
		PWGDescriptorSetLayout(const PWGDescriptorSetLayout&) = delete;
		PWGDescriptorSetLayout& operator=(const PWGDescriptorSetLayout&) = delete;

		VkDescriptorSetLayout getDescriptorSetLayout() const { return descriptorSetLayout; }

	private:
		PWGDevice& device;
		VkDescriptorSetLayout descriptorSetLayout;
		std::unordered_map<uint32_t, VkDescriptorSetLayoutBinding> bindings;

		friend class PWGDescriptorWriter;
	};

	class PWGDescriptorPool {
	public:
		class Builder {
		public:
			Builder(PWGDevice& device) : device{ device } {}

			Builder& addPoolSize(VkDescriptorType descriptorType, uint32_t count);
			Builder& setPoolFlags(VkDescriptorPoolCreateFlags flags);
			Builder& setMaxSets(uint32_t count);
			std::unique_ptr<PWGDescriptorPool> build() const;

		private:
			PWGDevice& device;
			std::vector<VkDescriptorPoolSize> poolSizes{};
			uint32_t maxSets = 1000;
			VkDescriptorPoolCreateFlags poolFlags = 0;
		};

		PWGDescriptorPool(
			PWGDevice& device,
			uint32_t maxSets,
			VkDescriptorPoolCreateFlags poolFlags,
			const std::vector<VkDescriptorPoolSize>& poolSizes);
		~PWGDescriptorPool();
		PWGDescriptorPool(const PWGDescriptorPool&) = delete;
		PWGDescriptorPool& operator=(const PWGDescriptorPool&) = delete;

		bool allocateDescriptor(
			const VkDescriptorSetLayout descriptorSetLayout, VkDescriptorSet& descriptor) const;

		void freeDescriptors(std::vector<VkDescriptorSet>& descriptors) const;

		void resetPool();

	private:
		PWGDevice& device;
		VkDescriptorPool descriptorPool;

		friend class PWGDescriptorWriter;
	};

	class PWGDescriptorWriter {
	public:
		PWGDescriptorWriter(PWGDescriptorSetLayout& setLayout, PWGDescriptorPool& pool);

		PWGDescriptorWriter& writeBuffer(uint32_t binding, VkDescriptorBufferInfo* bufferInfo);
		PWGDescriptorWriter& writeImage(uint32_t binding, VkDescriptorImageInfo* imageInfo);

		bool build(VkDescriptorSet& set);
		void overwrite(VkDescriptorSet& set);

	private:
		PWGDescriptorSetLayout& setLayout;
		PWGDescriptorPool& pool;
		std::vector<VkWriteDescriptorSet> writes;
	};
}
