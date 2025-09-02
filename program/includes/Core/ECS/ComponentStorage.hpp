#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>
#include <cassert>
#include <stdexcept>

#include "Core/ECS/Entity.hpp"
#include "Core/ECS/IComponent.hpp"

namespace core::ecs
{
    /**
     * Abstração para permitir manipulação genérica de pools
     */
    struct IPool
    {
        virtual ~IPool() = default;
        virtual void Erase(uint32_t id) = 0;
        virtual std::unique_ptr<IPool> GetClone() const = 0;
    };

    template<typename T>
    struct ComponentPool : IPool
    {
        std::unordered_map<uint32_t, T> data;

        void Erase(uint32_t id) override
        {
            data.erase(id);
        }

        std::unique_ptr<IPool> GetClone() const override
        {
            auto copy = std::make_unique<ComponentPool<T>>();
            copy->data = data;
            return copy;
        }
    };

    /**
     * @brief: Armazena e administra pools de componentes
     * 
     * Cada tipo T tem sua própria pool (mapa id -> componente).
     * O acesso é feito dinamicamente via type_index.
     */
    class ComponentStorage
    {
    public:
        // Adiciona um componente por tipo
        template<typename T, typename ...Args>
        T& AddComponent(uint32_t id, Args&&... args);

        // Captura um componente por tipo
        template<typename T>
        T* GetComponent(uint32_t id);

        // Verifica se a entidade possui um componente
        template<typename T>
        bool HasComponent(uint32_t id);

        // Remove o componente
        template<typename T>
        void RemoveComponent(uint32_t id);

        // Consulta de pool
        template<typename T>
        std::unordered_map<uint32_t, T>& GetPool();

        // Remove entidade de todas as pools
        void RemoveIDFromPools(uint32_t id)
        {
            for (auto& pair : m_ComponentPools)
			{
			    pair.second->Erase(id);
			}
        }

        // Clona as pools diretamente em m_ComponentPools.
        inline void CopyPools(const std::unordered_map<std::type_index, std::unique_ptr<IPool>>& original)
        {
            m_ComponentPools.clear();

            for (const auto& [type, pool] : original) 
            {
                m_ComponentPools[type] = pool->GetClone(); // polimórfico
            }
        }

        // Faz uma cópia de componentstorage, já clonando as pools.
        inline ComponentStorage GetCopy() const 
        {
            ComponentStorage copy;
            copy.CopyPools(m_ComponentPools);
            return copy;
        }

    private:
        // Um pool de componentes por tipo
        std::unordered_map<std::type_index, std::unique_ptr<IPool>> m_ComponentPools;
    };

    // Implementações
    template<typename T, typename... Args>
    T& ComponentStorage::AddComponent(uint32_t id, Args&&... args)
    {
        auto& pool = GetPool<T>();
        auto [it, inserted] = pool.try_emplace(id, std::forward<Args>(args)...);
        if (!inserted)
        {
            throw std::runtime_error("Não foi possível incluir componente: " + std::string(typeid(T).name()));
        }
        return it->second;
    }

    template<typename T>
    T* ComponentStorage::GetComponent(uint32_t id)
    {
        auto& pool = GetPool<T>();
        auto it = pool.find(id);
        return (it != pool.end()) ? &it->second : nullptr;
    }

    template<typename T>
    bool ComponentStorage::HasComponent(uint32_t id)
    {
        auto& pool = GetPool<T>();
        return pool.find(id) != pool.end();
    }

    template<typename T>
    void ComponentStorage::RemoveComponent(uint32_t id)
    {
        auto& pool = GetPool<T>();
        pool.erase(id);
    }

    template<typename T>
    std::unordered_map<uint32_t, T>& ComponentStorage::GetPool()
    {
        std::type_index typeIndex(typeid(T));
        auto it = m_ComponentPools.find(typeIndex);

        if (it == m_ComponentPools.end())
        {
            auto pool = std::make_unique<ComponentPool<T>>();
            auto& ref = pool->data;
            m_ComponentPools[typeIndex] = std::move(pool);
            return ref;
        }

        auto* pool = static_cast<ComponentPool<T>*>(it->second.get());
        return pool->data;
    }
}
