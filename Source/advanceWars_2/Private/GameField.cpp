// Source/advanceWars_2/Private/GameField.cpp

#include "./GameField.h"

#include "AW_GameMode.h"
#include "IO/IoContainerHeader.h"

// Sets default values
AGameField::AGameField() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	ObstaclesPercentage = 25;
	Size = 25;
	TileSize = 150.0f;
}

void AGameField::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AGameField::ResetField() {
	
	OnResetEvent.Broadcast();
	GenerateObstacles();
}

void AGameField::GenerateField() {
	for (int32 i = 0; i < Size; i++) {
		for (int32 j = 0; j < Size; j++) {
			FVector Location = GetRelativeLocationByXYPosition(i, j);
			ATile* Obj;
			if((i + j) % 2 == 0) {
				Obj = GetWorld()->SpawnActor<ATile>(TileClass_Odd, Location, FRotator::ZeroRotator);
			} else {
				Obj = GetWorld()->SpawnActor<ATile>(TileClass_Even, Location, FRotator::ZeroRotator);
			}
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetPosition(i, j);
			Tiles.Add(Obj);
			TileMap.Add(FVector2D(i, j), Obj);
		}
	}
}



void AGameField::GenerateObstacles()
{
	CurrentNode = new FTileNode(GetRandomTile());
	TreeCardinality++;
	while (RandomStep());
	FTileNode* RandomLeaf = SelectRandomLeaf();
	PlaceObstacle(RandomLeaf->Tile);
	TreeCardinality = 0;
}


FVector2D AGameField::GetPosition(const FHitResult& Hit) {
	return FVector2D(0, 0);
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const {
	return TileSize * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const {
	return FVector2D(Location[0], Location[1]) / TileSize;
}

ATile* AGameField::GetTileByXYPosition(const FVector2D) const
{
// CONTINUE HERE
}

ATile* AGameField::GetRandomEmptyTile()
{
	TArray<ATile*> EmptyTiles;
	for (ATile* Tile : Tiles) if (Tile->GetStatus() == ETileStatus::EMPTY) EmptyTiles.Add(Tile);
	return EmptyTiles[FMath::Rand() % EmptyTiles.Num()];
}

TArray<ATile*> AGameField::GetReachableTiles(ATile* DepartureTile, int32 TileDistance, bool bIgnoreObstacles)
{
	TArray<ATile*> ReachableTiles;
	// TODO
	return ReachableTiles;
}

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	UE_LOG(LogTemp, Warning, TEXT("AGameField::BeginPlay() called!"));
}

ATile* AGameField::GetRandomTile()
{
	return Tiles[FMath::RandRange(0, Tiles.Num() - 1)];
}

bool AGameField::RandomStep()
{
	TArray<ATile*> Neighbors = CurrentNode->Tile->GetNeighbors();
	TArray<ATile*> UnvisitedNeighbors;
	TArray<ATile*> ConnectedNeighbors;
	for (ATile* Neighbor : Neighbors)
	{
		if (!Neighbor->IsVisited()) UnvisitedNeighbors.Add(Neighbor);
		else if (CurrentNode->IsConnectedTo(Neighbor->TileNode)) ConnectedNeighbors.Add(Neighbor);
	}
	if (UnvisitedNeighbors.Num() > 0 && FMath::RandRange(0, 100) > 50 * (1 - FMath::Exp((-1) * ObstacleSpreadness)))
	{
		FTileNode* OldNode = CurrentNode;
		CurrentNode = new FTileNode(UnvisitedNeighbors[FMath::RandRange(0, UnvisitedNeighbors.Num() - 1)], OldNode);
		OldNode->AddChild(CurrentNode);
		TreeCardinality++;
		CurrentNode->Tile->TileNode = CurrentNode;
	} else
	{
		CurrentNode = ConnectedNeighbors[FMath::RandRange(0, ConnectedNeighbors.Num() - 1)]->TileNode;
		if (TreeCardinality == Tiles.Num()) return false;
	}
	return true;
}

FTileNode* AGameField::SelectRandomLeaf()
{
	// CONTINUE HERE , poi... hai un riferimento all'albero? forse salva la root
}

void AGameField::PlaceObstacle(ATile* Tile)
{
}


