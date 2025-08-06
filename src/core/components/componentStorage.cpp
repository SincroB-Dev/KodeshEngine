#include "componentStorage.h"

namespace core
{
    namespace components
    {
        template<typename T>
        void ComponentStorage::AddComponent(EntityID entity, T component)
        {
            auto& map = GetConmponentMap<T>();
            map[entity] = std::move(component);
        }

        template<typename T>
        T* ComponentStorage::GetComponent(EntityID entity)
        {
            auto& map = GetComponentMap<T>();
            auto it = map.find(entity);
            if (it != map.end())
            {
                return it->second();
            }
            return nullptr;
        }

        template<typename T>
        bool ComponentStorage::HasComponent(EntityID entity)
        {
            auto& map = GetComponentMap<T>();
            return map.find(entity) != map.end();
        }

        template<typename T>
        void ComponentStorage::RemoveComponent(EntityID entity)
        {
            auto& map = GetComponentMap<T>();
            map.erase(id);
        }

        template<typename T> 
        std::unordered_map<EntityID, T>& ComponentStorage::GetComponentMap()
        {
            static std::unordered_map<EntityID, T> map;
            return map;
        }
    }
}