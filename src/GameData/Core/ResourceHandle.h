#pragma once

class ResourceHandle
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
