#pragma once
#include <type_traits>


namespace proto {

	template <typename TTypeToReference>
	using Reference = typename std::conditional<
		std::is_void<TTypeToReference>::value,
		void,
		TTypeToReference&
	>::type;

}