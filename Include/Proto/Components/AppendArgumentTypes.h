#pragma once
#include "Proto/Components/AppendArgumentTypesTypeContainer.h"


namespace proto::components {

	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	using AppendArgumentTypes = typename AppendArgumentTypesTypeContainer<TBaseSignature, TArgumentTypesToAppend...>::type;

}