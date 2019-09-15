#pragma once

class ResourceHandle
{
public:
	typedef int IndexType;

public:
	ResourceHandle() = default;
	explicit ResourceHandle(IndexType index) : ResourceIndex(index) {}

	bool isValid() const { return ResourceIndex != InvalidResourceIndex; }

	static const IndexType InvalidResourceIndex = -1;
	IndexType ResourceIndex = InvalidResourceIndex;
};
