#pragma once

#include <vector>

namespace VectorUtils
{
	template <typename T>
	void RemoveIndexes(std::vector<T>& inOutVector, const std::vector<size_t>& sortedIndexesToRemove)
	{
		if (sortedIndexesToRemove.empty())
		{
			return;
		}

		const size_t vectorSize = inOutVector.size();
		const size_t indexesCount = sortedIndexesToRemove.size();
		size_t skippedIndexes = 0;
		for (size_t i = sortedIndexesToRemove[0] + 1, j = 1; i < vectorSize; ++i)
		{
			if (j < indexesCount && i == sortedIndexesToRemove[j])
			{
				++j;
				while (j < indexesCount && sortedIndexesToRemove[j] == i)
				{
					++j;
					++skippedIndexes;
				}
			}
			else
			{
				inOutVector[i - j + skippedIndexes] = std::move(inOutVector[i]);
			}
		}

		inOutVector.erase(inOutVector.begin() + (vectorSize - indexesCount + skippedIndexes), inOutVector.end());
	}

	template <typename T>
	void RemoveIndexesFast(std::vector<T>& inOutVector, const std::vector<size_t>& sortedIndexesToRemove)
	{
		const size_t vectorSize = inOutVector.size();
		const size_t indexesCount = sortedIndexesToRemove.size();
		size_t previousIndex = std::numeric_limits<size_t>::max();
		size_t skippedIndexes = 0;
		for (size_t i = 0; i < indexesCount; ++i)
		{
			size_t index = sortedIndexesToRemove[i];
			if (index == previousIndex)
			{
				++skippedIndexes;
				continue;
			}

			inOutVector[index] = std::move(inOutVector[vectorSize - i + skippedIndexes - 1]);
			previousIndex = index;
		}

		inOutVector.erase(inOutVector.begin() + (vectorSize - indexesCount + skippedIndexes), inOutVector.end());
	}
}
