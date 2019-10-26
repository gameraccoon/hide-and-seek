#pragma once

#include <type_traits>

class [[nodiscard]] ResourceHandle
{
public:
	using IndexType = int;

public:
	ResourceHandle() = default;
	explicit ResourceHandle(IndexType index) : ResourceIndex(index) {}

	bool isValid() const { return ResourceIndex != InvalidResourceIndex; }

	static const IndexType InvalidResourceIndex = -1;
	IndexType ResourceIndex = InvalidResourceIndex;
};

static_assert(std::is_trivially_copyable<ResourceHandle>(), "ResourceHandle should be trivially copyable");
