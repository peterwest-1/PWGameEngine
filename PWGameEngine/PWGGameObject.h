#pragma once
#include "PWGModel.h"
#include <glm/gtc/matrix_transform.hpp>
#include <memory>
#include <unordered_map>
namespace PWG {

	struct TransformComponent
	{
		glm::vec3 translation{};
		glm::vec3 scale{ 1.f, 1.f, 1.f };
		glm::vec3 rotation{};

		glm::mat4 mat4();
		glm::mat3 normalMatrix();
	};

	struct RigidBodyComponent
	{
		glm::vec3 velocity;
		float mass{ 1.f };
	};

	struct PointLightComponent {
		float lightIntensity = 1.f;
	};

	class PWGGameObject
	{
	public:
		using id_t = unsigned int;
		using Map = std::unordered_map<id_t, PWGGameObject>;

		static PWGGameObject createGameObject() {
			static id_t currentID = 0;
			return PWGGameObject{ currentID++ };
		}

		static PWGGameObject makePointLight(
			float intensity = 10.f, float radius = 0.1f, glm::vec3 color = glm::vec3(1.f));

		PWGGameObject(const PWGGameObject&) = delete;
		PWGGameObject& operator=(const PWGGameObject&) = delete;

		PWGGameObject(PWGGameObject&&) = default;
		PWGGameObject& operator=(PWGGameObject&&) = default;

		id_t getID() const { return id; }


		glm::vec3 color{};

		TransformComponent transform{};
		RigidBodyComponent rigidBody{};

		std::shared_ptr<PWGModel> model{};
		std::unique_ptr<PointLightComponent> pointLight = nullptr;



	private:
		PWGGameObject(id_t objectID) : id{ objectID } {

		}

		id_t id;

	};

}
