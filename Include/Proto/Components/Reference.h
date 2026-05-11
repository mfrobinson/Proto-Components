#pragma once
#include "Proto/Components/ReferenceTypeContainer.h"


namespace proto {

	template <typename TTypeToReference>
	using Reference = typename ReferenceTypeContainer<TTypeToReference>::type;

}