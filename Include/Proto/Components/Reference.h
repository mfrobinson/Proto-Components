#pragma once
#include "Proto/Components/ReferenceTypeContainer.h"


namespace proto::components {

	/**
	* @brief Helper alias to easily convert a type into an lvalue reference.
	* 
	* This alias simplifies the usage of ReferenceTypeContainer by directly exposing
	* the transformed type. If a void type is passed, it safely evaluates back to
	* void rather than causing a compilation failure.
	* 
	* @tparam TTypeToReference The underlying type to be converted.
	* 
	* @note Internal mapping mechanics are handled by ReferenceTypeContainer.
	*/
	template <typename TTypeToReference>
	using Reference = typename ReferenceTypeContainer<TTypeToReference>::type;

}