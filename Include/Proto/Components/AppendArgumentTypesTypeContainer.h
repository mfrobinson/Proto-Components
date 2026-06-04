#pragma once
#include "Proto/Components/AppendArgumentTypesTypeContainer.h"


namespace proto::components {

	/**
	* @brief Recursive template which appends a single or multiple types to a function signature type's arguments.
	*
	* This template appends a single or multiple types to a function signature type. Any argument types
	* to append that are the void type are ignored. The result of appending these argument types is accessed
	* through the type field defined inside this container.
	*
	* @tparam TBaseSignature The base function signature type which is to have the other supplied argument types appended to it.
	* @tparam TArgumentTypesToAppend The typename parameter pack containing the argument types to append to the base function signature.
	*/
	template <typename TBaseSignature, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer;

	/**
	* @brief Terminating base case for the recursive template.
	* 
	* This specialisation is chosen when the TArgumentTypesToAppend parameter pack is empty. Hence the defined type field in this struct is
	* the supplied base function signature.
	*
	* @tparam TBaseSignature The base function signature type which is now the result of appending all of the supplied extra argument
	*	types to the original base function signature.
	*/
	template <typename TBaseSignature>
	struct AppendArgumentTypesTypeContainer<TBaseSignature> {
		using type = TBaseSignature;
	};

	/**
	* @brief Specialisation to handle ignoring void argument types.
	* 
	* This specialisation unpacks a void type from the TArgumentTypesToAppend parameter pack. Since void argument types are
	* skipped, the rest of the parameter pack is simply passed back to this recursive template.
	* 
	* @tparam TBaseSignature The base function signature type which is to have the other supplied argument types appended to it.
	* @tparam TArgumentTypesToAppend The typename parameter pack containing the rest of the argument types to append to the base
	*	function signature.
	*/
	template <typename TReturnType, typename... TExistingArgumentTypes, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), void, TArgumentTypesToAppend...> {
		using type = typename AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), TArgumentTypesToAppend...>::type;
	};

	/**
	* @brief Append a non-void argument type to a function signature.
	* 
	* This specialisation unpacks a non-void type from the TArgumentTypesToAppend parameter pack. The function signature is deconstructed
	* and the argument type is appended to the already existing argument types. The new function signature type and the rest of the argument
	* types to append are passed back to this recursive template and the resulting type field is then used as this struct's type field.
	* 
	* @tparam TReturnType The return type of the base function signature type.
	* @tparam TExistingArgumentTypes The typename parameter pack containing the existing argument types of the base function signature.
	* @tparam TFirstArgumentTypeToAdd The typename of the first unpacked type from the TArgumentTypesToAppend typename parameter pack.
	* @tparam TArgumentTypesToAppend The typename parameter pack containing the rest of the argument types to append to the base function
	*	signature type.
	*/
	template <typename TReturnType, typename... TExistingArgumentTypes, typename TFirstArgumentTypeToAdd, typename... TArgumentTypesToAppend>
	struct AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes...), TFirstArgumentTypeToAdd, TArgumentTypesToAppend...> {
		using type = typename AppendArgumentTypesTypeContainer<TReturnType(TExistingArgumentTypes..., TFirstArgumentTypeToAdd), TArgumentTypesToAppend...>::type;
	};

}