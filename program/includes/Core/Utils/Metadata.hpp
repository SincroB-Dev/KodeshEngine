#pragma once

#include "Core/Utils/Span.hpp"

#include <cstring>

namespace core
{
	enum class MetaType 
	{
		Flow, // Tipo de transição de dados, utilizado mais em nodes.

	    Int,
	    Float,
	    String,
	    Bool,
	    Enum,

	    Object,
		Function,
		Delegate
	};

	struct EnumEntry 
	{
	    const char* Name;
	    int Value;
	};

	struct TypeDescriptor
	{
	    MetaType Type;
	    const char* TypeName;

	    // Para enums
	    Span<const EnumEntry> EnumEntries;
	};

	/**
	 * @brief Registro de enumerador.
	 **/
	template<typename T>
	struct EnumRegistry; // default vazio

	template<typename T>
	inline constexpr TypeDescriptor MakeEnumDescriptor(const char* typename_) 
	{
	    return TypeDescriptor
	    {
	        MetaType::Enum,
	        typename_,
	        EnumRegistry<T>::entries
	    };
	}

	/**
	 * @brief Converte um enumerador para string. 
	 **/
	inline const char* EnumToString(const TypeDescriptor& descriptor, int value)
	{
		if (descriptor.Type != MetaType::Enum) return nullptr;
		for (auto& e : descriptor.EnumEntries)
		{
			if (e.Value == value)
			{
				return e.Name;
			}
		}
		return nullptr;
	}

	/**
	 * @brief Converte uma string para enumerador. 
	 **/
	inline bool StringToEnum(const TypeDescriptor& descriptor, const char* str, int& value)
	{
		if (descriptor.Type != MetaType::Enum) return false;
		for (auto& e : descriptor.EnumEntries)
		{
			if (strcmp(str, e.Name) == 0)
			{
				value = e.Value;
				return true;
			}
		}	
		return false;
	}
}