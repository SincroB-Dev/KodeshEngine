#include "Core/Events/EventDispatcher.hpp"

namespace core::events
{
	utils::UniqueIDGen EventDispatcher::s_NextID = utils::UniqueIDGen();

	void EventDispatcher::Unregister(void* owner)
	{
		for (auto& [type, vec] : m_Callbacks) {
	        vec.erase(std::remove_if(vec.begin(), vec.end(),
	            [&](const EventEntry& entry) {
	                return entry.Owner == owner;
	            }),
	            vec.end()
            );
	    }
	}

	void EventDispatcher::Unregister(utils::UniqueID id)
	{
		for (auto& [type, vec] : m_Callbacks) {
	        vec.erase(std::remove_if(
	        	vec.begin(),  vec.end(),
	            [&](const EventEntry& entry) {
	                return entry.UID.ID == id.ID;
	            }),
	            vec.end()
            );
	    }
	}

	void EventDispatcher::Dispatch(Event& event)
	{
		auto it = m_Callbacks.find(typeid(event));
		if (it != m_Callbacks.end())
		{
			for (auto& evEntry : it->second)
			{
				// Com isso dá para marcar event.Handled = true quando clicar em um botão
				// evitando passar para scene.
				if (event.Handled) break; // Interrompe se consumido;

				evEntry.Callback(event);
			}
		}
	}
}