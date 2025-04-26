// TileNode.h
#pragma once

#include "CoreMinimal.h"

class ATile;

class FTileNode
{
public:
	ATile* Tile = nullptr;
	FTileNode* Parent = nullptr;
	TArray<FTileNode*> Children;

	FTileNode() = default;

	FTileNode(ATile* InTile, FTileNode* InParent = nullptr)
		: Tile(InTile), Parent(InParent)
	{}

	void AddChild(FTileNode* Child)
	{
		if (Child)
		{
			Child->Parent = this;
			Children.Add(Child);
		}
	}

	bool IsConnectedTo(const FTileNode* Other) const
	{
		return Other->Parent == this || Parent == Other;
	}

	bool operator==(const FTileNode* Other) const
	{
		return Tile == Other->Tile; // FIXME ? funzionerà? boh
	}
};
