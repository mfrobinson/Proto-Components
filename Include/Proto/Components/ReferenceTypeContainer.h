#pragma once


namespace proto::components {

	/**
	* @brief A container utility that transforms a standard type into an lvalue reference type.
	* 
	* This template provides a type-safe way to convert any valid type into its respective
	* reference type. The resulting type is accessed through the internal type field.
	* 
	* @tparam TTypeToReference The underlying type to be converted into a reference.
	*/
	template <typename TTypeToReference>
	struct ReferenceTypeContainer {
		using type = TTypeToReference&;
	};

	/**
	* @brief Specialisation to handle the conversion of void types.
	*
	* This specialisation handles cases where the type argument is void. Since references
	* to void are illegal in C++, it safely maps a void input to a void output type to prevent
	* compilation failures.
	*/
	template <>
	struct ReferenceTypeContainer<void> {
		using type = void;
	};

}