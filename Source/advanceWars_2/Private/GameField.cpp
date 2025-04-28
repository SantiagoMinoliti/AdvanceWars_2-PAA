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
			FActorSpawnParameters SpawnParams;
            SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
			if((i + j) % 2 == 0) {
				Obj = GetWorld()->SpawnActor<ATile>(TileClass_Odd, Location, FRotator::ZeroRotator, SpawnParams);
			} else {
				Obj = GetWorld()->SpawnActor<ATile>(TileClass_Even, Location, FRotator::ZeroRotator, SpawnParams);
			}
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetPosition(i, j);
			Tiles.Add(Obj);
			TileMap.Add(FVector2D(i, j), Obj);
		}
	}
	GenerateObstacles();
}



void AGameField::GenerateObstacles()
{
    int32 ObstaclesPlaced = 0;
    int32 TargetObstaclesNum = Size * Size * ObstaclesPercentage / 100;

    UE_LOG(LogTemp, Log, TEXT("[ObstacleGen] Starting obstacle generation: target = %d obstacles (grid size = %dx%d, percentage = %d%%)"),
        TargetObstaclesNum, Size, Size, ObstaclesPercentage);

    while (ObstaclesPlaced < TargetObstaclesNum)
    {
        TArray<ATile*> ToUnmark;
        ATile* CandidateTile = nullptr;

        UE_LOG(LogTemp, VeryVerbose, TEXT("[ObstacleGen] Starting new placement attempt #%d"), ObstaclesPlaced + 1);

        int32 TryCount = 0;

        do
        {
            TryCount++;

            CandidateTile = GetRandomEmptyUnmarkedTile();
            if (!CandidateTile)
            {
                UE_LOG(LogTemp, Error, TEXT("[ObstacleGen] No empty unmarked tiles available after %d tries. Aborting obstacle generation!"), TryCount);
                return;
            }
        	UE_LOG(LogTemp, Log, TEXT("[ObstacleGen] Try #%d"), TryCount);

            FVector2D Pos = CandidateTile->GetPosition();
            UE_LOG(LogTemp, VeryVerbose, TEXT("[ObstacleGen] Try #%d: Picked candidate tile at "), TryCount)

            CandidateTile->Mark();
            ToUnmark.Add(CandidateTile);

            UE_LOG(LogTemp, VeryVerbose, TEXT("[ObstacleGen] Marked tile (%d, %d) for validation."), 
                static_cast<int32>(Pos.X), static_cast<int32>(Pos.Y));

        } while (!TryPlaceObstacle(CandidateTile));  // Continua a provare finché TryPlaceObstacle fallisce

        // Se siamo usciti dal do-while, significa che abbiamo una tile dove mettere l'ostacolo
        FVector2D FinalPos = CandidateTile->GetPosition();
        PlaceObstacle(CandidateTile);
        ObstaclesPlaced++;

        UE_LOG(LogTemp, Log, TEXT("[ObstacleGen] Obstacle #%d placed at (%d, %d) after %d tries."), 
            ObstaclesPlaced, static_cast<int32>(FinalPos.X), static_cast<int32>(FinalPos.Y), TryCount);

        for (ATile* TileToUnmark : ToUnmark)
        {
            TileToUnmark->Unmark();
        }

        UE_LOG(LogTemp, VeryVerbose, TEXT("[ObstacleGen] Unmarked %d temporary marked tiles."), ToUnmark.Num());
    }

    UE_LOG(LogTemp, Log, TEXT("[ObstacleGen] Finished placing all obstacles: %d obstacles placed successfully."), ObstaclesPlaced);
}



FVector2D AGameField::GetPosition(const FHitResult& Hit) {
	// TODO
	return FVector2D(0, 0);
}

FVector AGameField::GetRelativeLocationByXYPosition(const int32 InX, const int32 InY) const {
	return TileSize * FVector(InX, InY, 0);
}

FVector2D AGameField::GetXYPositionByRelativeLocation(const FVector& Location) const {
	return FVector2D(Location[0], Location[1]) / TileSize;
}

ATile* AGameField::GetTileByXYPosition(const FVector2D Position) const
{
	if (Position.X >= 0 && Position.X < Size && Position.Y >= 0 && Position.Y < Size) return TileMap[Position];
	else return nullptr;
}

ATile* AGameField::GetRandomEmptyTile()
{
	TArray<ATile*> EmptyTiles;
	for (ATile* Tile : Tiles) if (Tile->GetStatus() == ETileStatus::EMPTY) EmptyTiles.Add(Tile);
	if (EmptyTiles.Num() == 0) return nullptr;
	return EmptyTiles[FMath::Rand() % EmptyTiles.Num()];
}

ATile* AGameField::GetRandomEmptyUnmarkedTile()
{
	TArray<ATile*> EmptyUnmarkedTiles;

	for (ATile* Tile : Tiles)
	{
		if (Tile->GetStatus() == ETileStatus::EMPTY && !Tile->IsMarked())
		{
			EmptyUnmarkedTiles.Add(Tile);
		}
	}

	UE_LOG(LogTemp, Log, TEXT("Found %d Empty and Unmarked Tiles."), EmptyUnmarkedTiles.Num());

	if (EmptyUnmarkedTiles.Num() == 0)
	{
		UE_LOG(LogTemp, Warning, TEXT("GetRandomEmptyUnmarkedTile: No suitable tiles found!"));
		return nullptr;
	}

	int32 RandomIndex = FMath::Rand() % EmptyUnmarkedTiles.Num();
	ATile* ChosenTile = EmptyUnmarkedTiles[RandomIndex];

	FVector2D ChosenPos = ChosenTile->GetPosition();
	UE_LOG(LogTemp, Log, TEXT("Randomly selected Tile at (%d, %d)"),
		static_cast<int32>(ChosenPos.X), static_cast<int32>(ChosenPos.Y));

	return ChosenTile;
}


bool AGameField::TryPlaceObstacle(ATile* CandidateTile)
{
	// Reset Visited su tutte le tile
	for (ATile* Tile : Tiles)
	{
		if (Tile)
		{
			Tile->Visited = false;
		}
	}

	// Trova una tile vuota come partenza
	ATile* StartTile = GetRandomEmptyTile();

	if (!StartTile)
	{
		
		return true;
	}

	// DFS stack
	TArray<ATile*> Stack;
	Stack.Push(StartTile);
	StartTile->Visited = true;

	while (Stack.Num() > 0)
	{
		ATile* Current = Stack.Pop();

		for (ATile* Neighbor : Current->GetNeighbors())
		{
			if (Neighbor && Neighbor->GetStatus() == ETileStatus::EMPTY && !Neighbor->Visited /*&& Neighbor->GetPosition() == CandidateTile->GetPosition()*/)
			{
				Neighbor->Visited = true;
				Stack.Push(Neighbor);
			}
		}
	}

	// Dopo DFS, controlliamo se tutte le tile vuote sono state visitate
	for (ATile* Tile : Tiles)
	{
		if (Tile && Tile->GetStatus() == ETileStatus::EMPTY && !Tile->Visited)
		{
			return false; // Trovata una casella vuota NON raggiunta
		}
	}

	return true; // Tutte le caselle vuote sono collegate
}

void AGameField::PlaceObstacle(ATile* Tile)
{
	if (!Tile)
	{
		UE_LOG(LogTemp, Warning, TEXT("PlaceObstacle called with nullptr Tile!"));
		return;
	}

	FVector2D Position = Tile->GetPosition();
	FVector Location = GetRelativeLocationByXYPosition(Position.X, Position.Y);

	UE_LOG(LogTemp, Log, TEXT("Placing obstacle at Tile position (%d, %d), world location (%.2f, %.2f, %.2f)"),
		static_cast<int32>(Position.X), static_cast<int32>(Position.Y),
		Location.X, Location.Y, Location.Z);

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
    
	ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass_Obstacle, Location, FRotator::ZeroRotator, SpawnParams);
	if (!Obj)
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to spawn obstacle tile actor at (%d, %d)!"), 
			static_cast<int32>(Position.X), static_cast<int32>(Position.Y));
		return;
	}

	const float TileScale = TileSize / 100;
	Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.4f));
	Obj->SetPosition(Position.X, Position.Y);

	Tile->SetStatus(ETileStatus::OBSTACLE);

	UE_LOG(LogTemp, Log, TEXT("Obstacle placed successfully at (%d, %d)"), 
		static_cast<int32>(Position.X), static_cast<int32>(Position.Y));
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




