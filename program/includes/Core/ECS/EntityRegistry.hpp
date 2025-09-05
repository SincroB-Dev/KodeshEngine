#pragma once

#include <vector>
#include <typeindex>
#include <nlohmann/json.hpp>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/IComponent.hpp"
#include "Core/ECS/ComponentStorage.hpp"
#include "Core/Utils/UniqueIDGen.hpp"

#include "Core/Serialization/PersistenceFwd.hpp"

#include "Core/Events/EventDispatcher.hpp"
#include "Core/Input/InputManager.hpp"

namespace core::systems
{
    class SceneManager;
}

namespace core::ecs
{
    class EntityRegistry
    {
    public:
        // Adiciona um componente por tipo
        template<typename T, typename ...Args>
        T& AddComponent(Entity e, Args&&... args)
        {
            return m_ComponentStorage.AddComponent<T>(e.ID, std::forward<Args>(args)...);
        }

        // Captura um componente por tipo
        template<typename T>
        T* GetComponent(Entity e)
        {
            return m_ComponentStorage.GetComponent<T>(e.ID);
        }

        // Verifica se a entidade possui um componente
        template<typename T>
        bool HasComponent(Entity e)
        {
            return m_ComponentStorage.HasComponent<T>(e.ID);
        }

        // Ferifica se a entidade possuí um componente, porém essa verificação 
        // é utilizando o id da classe, ao invez de templates.
        bool HasComponent(std::type_index Tidx, Entity e)
        {
            return m_ComponentStorage.HasComponent(Tidx, e.ID);
        }

        // Remove o componente
        template<typename T>
        void RemoveComponent(Entity e)
        {
            m_ComponentStorage.RemoveComponent<T>(e.ID);
        }

        // Consulta de pool
        template<typename T>
        std::unordered_map<uint32_t, T>& GetPool()
        {
            return m_ComponentStorage.GetPool<T>(); 
        }

        // Cria uma nova entidade, reutilizando o ID se possível
        ecs::Entity CreateEntity();

        // Consulta entities
        const std::vector<Entity>& GetEntities() const;

        // Marca a entidade como destruída e libera o ID
        void DestroyEntity(Entity e);

        // Verifica se a entidade ainda é válida
        bool IsAlive(const Entity& e) const;

        // Cria um clone do entity registry.
        EntityRegistry GetCopy() const;

        EntityRegistry()
            : m_Entities(), m_NextID(), m_ComponentStorage() {}

    private:
        // construtor privado para clone
        EntityRegistry(const std::vector<Entity>& entities,
                       const ComponentStorage& storage,
                       const utils::UniqueIDGen& idGen)
            : m_Entities(entities)
            , m_NextID(idGen.GetCopy()) // inicializa manualmente
            , m_ComponentStorage(storage.GetCopy())
        {}

        std::vector<Entity> m_Entities;

        utils::UniqueIDGen m_NextID;
        ComponentStorage m_ComponentStorage;

        // Tornar especializadores friend de classes para que seja mais simples a serialização/deserialização
        friend nlohmann::json serialization::persistence::SerializeSystem<systems::SceneManager>(const systems::SceneManager&);
    };
}
