#pragma once

#include "Core/Systems/SceneManager.hpp"

#include "Core/Serialization/PersistenceRegistry.hpp"

#include "Core/ECS/Entity.hpp"
#include "Core/Utils/UniqueIDGen.hpp"

#include <stdexcept>
#include <nlohmann/json.hpp>
#include <vector>

namespace core::serialization::persistence
{
	template<>
	inline nlohmann::json SerializeSystem<systems::SceneManager>(const systems::SceneManager& sm)
	{
		nlohmann::json project;
		nlohmann::json sceneList;

	    // Percorre pelas cenas, para fazer o armazenamento de entities e componentes.
		for (auto& scene : sm.m_Scenes)
		{
			// Guarda a cena que está ativa no projeto.
			if (sm.m_ActiveScene == scene.get())
			{
				project["active_scene"] = scene->GetName();
			}

			nlohmann::json jsonEntities = {};

			// Entities da cena. (Entities são apenas IDs que permitem leitura mais fácil de dados).
			auto& registry = scene->GetRegistry();
			auto& entities = registry.GetEntities();

			// Percorre por entities, para armazenar seu ID e seus componentes.
			for (auto& entt : entities)
			{
				nlohmann::json jsonComponents = {};

				// Serializa os componentes em json objects (jsonComponents)
				PersistenceRegistry::Instance()
					.SerializeComponents(entt, registry, jsonComponents);

				jsonEntities.push_back({
					{ "id", entt.ID },
					{ "generation", entt.Generation },
					{ "components", jsonComponents }
				});
			}

			nlohmann::json jsonUIDG;

			jsonUIDG["current"]     = registry.m_NextID.LastGeneratedID();
			jsonUIDG["generations"] = registry.m_NextID.GetGenerations();
			jsonUIDG["free"] = {};

			auto queue = registry.m_NextID.GetFreeList();

			for (;!queue.empty();) {
			    jsonUIDG["free"].push_back(queue.front());
			    queue.pop();
			}

			sceneList.push_back({
				{"name", scene->GetName()}, // nome da cena.
				{"entities", jsonEntities }, // entidades.
				{"cached_uidg", jsonUIDG } // backup do unique id generator
			});
		}

		project["scenes"] = sceneList;

	    return project;
	}

	template<>
	inline void DeserializeSystem<systems::SceneManager>(systems::SceneManager* sm, const nlohmann::json &data)
	{
		if (data.is_object())
		{
			for (auto& jsonScene : data["scenes"])
			{
				auto* scene = sm->AddScene(jsonScene["name"].get<std::string>());
				nlohmann::json cached_uidg = jsonScene["cached_uidg"];

				// Entities da cena. (Entities são apenas IDs que permitem leitura mais fácil de dados).
				auto& registry = scene->GetRegistry();

				for (auto& jsonEntity : jsonScene["entities"])
				{
					// Cria um entity baseado no id e geração vindos do json.
					auto entity = registry.CreateEntity(
						ecs::Entity(utils::UniqueID(jsonEntity["id"].get<uint32_t>(), jsonEntity["generation"].get<uint32_t>()))
					);

					serialization::PersistenceRegistry::Instance()
						.DeserializeComponents(entity, registry, jsonEntity["components"]);
				}

				std::queue<uint32_t> freeList;

				if (cached_uidg.contains("free") && !cached_uidg["free"].is_null())
				{
					nlohmann::json jsonFree = cached_uidg["free"];

					if (!jsonFree.is_array())
					{
						throw std::runtime_error("[<Project>]:scenes:[<Scene>]:cached_uidg:free deveria ser um array!");
					}

					std::vector<uint32_t> free = cached_uidg["free"].get<std::vector<uint32_t>>();

					for (auto id : cached_uidg["free"])
					{
						freeList.push(id);
					}
				}

				std::vector<uint32_t> generations;
				generations.clear();

				if (cached_uidg.contains("generations") && !cached_uidg["generations"].is_null())
				{
					nlohmann::json jsonGenerations = cached_uidg["generations"];

					if (!jsonGenerations.is_array())
					{
						throw std::runtime_error("[<Project>]:scenes:[<Scene>]:cached_uidg:generations deveria ser um array!");
					}

					generations = jsonGenerations.get<std::vector<uint32_t>>();
				}

				registry.m_NextID
						.Set(cached_uidg["current"].get<uint32_t>(), generations, freeList);
			}
		}
	}
}