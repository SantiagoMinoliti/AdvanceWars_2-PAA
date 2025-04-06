// Source/advanceWars_2/Private/GameField.cpp

#include "./GameField.h"

#include "IO/IoContainerHeader.h"

// Sets default values
AGameField::AGameField() {
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	Size = 25;
	TileSize = 1.5f;
}

void AGameField::OnConstruction(const FTransform& Transform) {
	Super::OnConstruction(Transform);
}

void AGameField::ResetField() {

}

void AGameField::GenerateField() {
	for (int32 i = 0; i < Size; i++) {
		for (int32 j = 0; j < Size; j++) {
			FVector Location = AGameField::GetRelativeLocationByXYPosition(i, j);
			ATile* Obj = GetWorld()->SpawnActor<ATile>(TileClass, Location, FRotator::ZeroRotator);
			const float TileScale = TileSize / 100;
			Obj->SetActorScale3D(FVector(TileScale, TileScale, 0.2));
			Obj->SetPosition(i, j);
			Tiles.Add(Obj);
			TileMap.Add(FVector2D(i, j), Obj);
		}
	}
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

// Called when the game starts or when spawned
void AGameField::BeginPlay()
{
	Super::BeginPlay();
	GenerateField();
}