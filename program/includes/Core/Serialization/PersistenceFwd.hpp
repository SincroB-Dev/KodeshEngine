#pragma once

#include <memory>
#include <nlohmann/json.hpp>

namespace core::serialization::persistence
{
	//-------------------------------
	// Componentes ECS
	//-------------------------------

	template<typename T>
	nlohmann::json SerializeComponent(const T&);

	template<typename T>
	T DeserializeComponent(const nlohmann::json&);
}