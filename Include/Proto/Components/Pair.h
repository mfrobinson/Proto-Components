#pragma once


namespace proto::components {

	/**
	* @brief A composition utility that combines two types into a single unit.
	* 
	* This template acts as a flexible data container for two elements. It is
	* designed to safely handle conditional compilation where either or both
	* elements are void.
	* 
	* @tparam TFirst The type of the first element in the pair.
	* @tparam TSecond The type of the second element in the pair.
	*/
	template <typename TFirst, typename TSecond>
	struct Pair {
		TFirst first;
		TSecond second;
	};

	/**
	* @brief Specialisation for pairs that do not require a second element.
	*
	* This structure is chosen when the TSecond template parameter is void. It
	* prevents compilation failures by completely omitting the second member variable.
	*
	* @tparam TFirst The type of the first element in the pair.
	*/
	template <typename TFirst>
	struct Pair<TFirst, void> {
		TFirst first;
	};

	/**
	* @brief Specialisation for pairs that do not require a first element.
	*
	* This structure is chosen when the TFirst template parameter is void. It
	* prevents compilation failures by completely omitting the first member variable.
	*
	* @tparam TSecond The type of the second element in the pair.
	*/
	template <typename TSecond>
	struct Pair<void, TSecond> {
		TSecond second;
	};

	/**
	* @brief Full specialisation for pairs that do not have either field.
	* 
	* This explicit specialisation is chosen when both template parameters are void,
	* resulting in an empty struct.
	*/
	template <>
	struct Pair<void, void> {
	};

}