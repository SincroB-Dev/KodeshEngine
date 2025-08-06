#pragma once

#include <unordered_map>
#include <typeindex>
#include <memory>

#include "../entity.h"

namespace core
{
    namespace components
    {
        class ComponentStorage
        {
        public:
            template<typename T> void AddComponent(EntityID entity, T component);
            template<typename T> T* GetComponent(EntityID entity);
            template<typename T> bool HasComponent(EntityID entity);
            template<typename T> void RemoveComponent(EntityID entity);

        private:
            template<typename T> std::unordered_map<EntityID, T>& GetComponentMap();
        };
    }
}