#pragma once


namespace proto::components {

	template <typename TTypeToReference>
	struct ReferenceTypeContainer {
		using type = TTypeToReference&;
	};

	template <>
	struct ReferenceTypeContainer<void> {
		using type = void;
	};

}