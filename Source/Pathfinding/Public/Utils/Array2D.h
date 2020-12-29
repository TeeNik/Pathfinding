#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"

template<typename InElementType>
class TArray2D
{
public:
	void Init(InElementType value, int x, int y);
	InElementType& Get(int x, int y);
	InElementType& Get(int index);
	int Num() const { return SizeX * SizeY; }

protected:
	TArray<InElementType> Array;

	int SizeX = 0;
	int SizeY = 0;
};

template <typename InElementType>
void TArray2D<InElementType>::Init(InElementType value, int x, int y)
{
	Array.Init(value, x * y);
	SizeX = x;
	SizeY = y;
}

template <typename InElementType>
InElementType& TArray2D<InElementType>::Get(int x, int y)
{
	return Array[x * SizeX + y];
}

template <typename InElementType>
InElementType& TArray2D<InElementType>::Get(int index)
{
	return Array[index];
}
