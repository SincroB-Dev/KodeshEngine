#pragma once

#include <cstddef> // size_t
#include <iterator>
#include <array>

#define ENUM_VALUE(name) { #name, static_cast<int>(T::name) }

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

	/**
	 * @brief Classe básica minimalista para Span.
	 **/
	template<typename T>
	class Span {
	public:
	    using element_type    = T;
	    using value_type      = typename std::remove_cv<T>::type;
	    using size_type       = std::size_t;
	    using pointer         = T*;
	    using reference       = T&;
	    using iterator        = T*;
	    using const_iterator  = const T*;

	    constexpr Span() noexcept : data_(nullptr), size_(0) {}
	    constexpr Span(T* ptr, size_type count) noexcept : data_(ptr), size_(count) {}
	    constexpr Span(T* first, T* last) noexcept : data_(first), size_(last - first) {}

	    template<size_t N>
		constexpr Span(std::array<value_type, N>& arr) noexcept
		    : data_(arr.data()), size_(N) {}

		// Construtor a partir de std::array const
		template<size_t N>
		constexpr Span(const std::array<value_type, N>& arr) noexcept
		    : data_(arr.data()), size_(N) {}

	    template<size_t N>
	    constexpr Span(T (&arr)[N]) noexcept : data_(arr), size_(N) {}

	    constexpr iterator begin() const noexcept { return data_; }
	    constexpr iterator end()   const noexcept { return data_ + size_; }

	    constexpr reference operator[](size_type idx) const { return data_[idx]; }
	    constexpr size_type size() const noexcept { return size_; }
	    constexpr bool empty() const noexcept { return size_ == 0; }
	    constexpr pointer data() const noexcept { return data_; }

	private:
	    T* data_;
	    size_type size_;
	};


	struct EnumEntry 
	{
	    const char* name;
	    int value;
	};

	struct TypeDescriptor
	{
	    MetaType type;
	    const char* typeName;

	    // Para enums
	    Span<const EnumEntry> enumEntries;
	};

	/**
	 * @brief Registro de enumerador.
	 **/
	template<typename T>
	struct EnumRegistry; // default vazio

	template<typename T>
	inline constexpr TypeDescriptor MakeEnumDescriptor(const char* typeName) 
	{
	    return TypeDescriptor
	    {
	        MetaType::Enum,
	        typeName,
	        EnumRegistry<T>::entries
	    };
	}
}