#pragma once
#include "Proto/Components/AppendArgumentTypesTypeContainer.h"


namespace proto::components {

	/**
	* @brief Helper alias to easily append one or more types to a function signature's arguments.
	*
	* This alias simplifies the usage of AppendArgumentTypesTypeContainer by directly exposing
	* the resulting type. If a void type is passed as one of the argument types, it is safely ignored
	* rather than causing a compilation failure.
	*
	* @tparam TBaseSignature The original function signature.
	* @tparam TArgumentTypesToAppend The types to add to the end of the function signature's arguments.
	*
	* @note Internal mapping mechanics are handled by AppendArgumentTypesTypeContainer.
	*/
	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	using AppendArgumentTypes = typename AppendArgumentTypesTypeContainer<TBaseSignature, TArgumentTypesToAppend...>::type;

}