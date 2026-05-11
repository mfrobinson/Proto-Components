#pragma once
#include "Proto/AppendArgumentTypesTypeContainer.h"


namespace proto {

	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	using AppendArgumentTypes = typename AppendArgumentTypesTypeContainer<TBaseSignature, TArgumentTypesToAppend...>::type;

}