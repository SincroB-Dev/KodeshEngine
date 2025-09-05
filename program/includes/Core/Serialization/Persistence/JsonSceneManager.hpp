#pragma once

#include "Core/Systems/SceneManager.hpp"

#include "Core/Serialization/SerializerRegistry/ComponentsRegistry.hpp"

#include <nlohmann/json.hpp>

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
				ComponentsRegistry::Instance()
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
}