#pragma once


namespace proto {

	template <typename TBase, typename TExtension>
	struct ExtendStruct {
		TBase base;
		TExtension extension;
	};

	template <typename TBase>
	struct ExtendStruct<TBase, void> {
		TBase base;
	};

	template <typename TExtension>
	struct ExtendStruct<void, TExtension> {
		TExtension extension;
	};

	template <>
	struct ExtendStruct<void, void> {
	};

}