#pragma once
#include "Proto/Components/AppendArgumentTypesTypeContainer.h"


namespace proto {

	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	using AppendArgumentTypes = typename AppendArgumentTypesTypeContainer<TBaseSignature, TArgumentTypesToAppend...>::type;

}