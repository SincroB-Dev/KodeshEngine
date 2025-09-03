#include "Core/Serialization/SaveSystem.hpp"
#include "Core/Utils/Version.hpp"
#include "Core/Events/SaveSystemEvent.hpp"

#include <nlohmann/json.hpp>

#include <fstream>
#include <iostream>

using json = nlohmann::json;

using namespace core::utils;

namespace core::serialization
{
	void SaveSystem::Save(const std::string& path)
	{
		json root;

		root["version"] = Current.ToString();
		
		events::SaveProjectEvent saveEvent(root["data"]);
		ke_Dispatcher.Dispatch(saveEvent);

		std::ofstream file(path);
        file << root.dump(4);
	}

	void SaveSystem::Load(const std::string& path)
	{
		std::ifstream file(path);
        json root;
        file >> root;

		events::LoadProjectEvent loadEvent(root["data"]);
		ke_Dispatcher.Dispatch(loadEvent);
    }
}