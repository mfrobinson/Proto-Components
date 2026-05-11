#pragma once
#include "Proto/ReferenceTypeContainer.h"


namespace proto {

	template <typename TTypeToReference>
	using Reference = typename ReferenceTypeContainer<TTypeToReference>::type;

}