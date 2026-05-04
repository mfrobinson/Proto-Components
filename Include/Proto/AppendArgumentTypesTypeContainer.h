#pragma once
#include "Proto/AppendArgumentTypesTypeContainer.h"


namespace proto {

	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer;

	template <typename TBaseSignature>
	struct AppendArgumentTypesTypeContainer {
		using type = TBaseSignature;
	};

	template <typename TReturnType, typename... TExistingArgumentTypes, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), void, TArgumentTypesToAppend...> {
		using type = typename AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), TArgumentTypesToAppend...>::type;
	};

	template <typename TReturnType, typename... TExistingArgumentTypes, typename TFirstArgumentTypeToAdd, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), TFirstArgumentTypeToAdd, TArgumentTypesToAppend...> {
		using type = typename AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes..., TFirstArgumentTypeToAdd), TArgumentTypesToAppend...>::type;
	};

}