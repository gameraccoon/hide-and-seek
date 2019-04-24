#pragma once

class ResourceHandle
{
public:
	typedef int IndexType;

	ResourceHandle() = default;
	// implicit conversation
	ResourceHandle(IndexType index) : ResourceIndex(index) {}

	bool isValid() const { return ResourceIndex != 0; }

	IndexType ResourceIndex = 0;
};
