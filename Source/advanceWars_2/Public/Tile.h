#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Tile.generated.h"

UENUM()
enum class ETileStatus : uint8
{
	EMPTY UMETA(DisplayName = "Empty"),
	OBSTACLE UMETA(DisplayName = "Obstacle"),
	SANTA UMETA(DisplayName = "Santa"),
	BERNARD UMETA(DisplayName = "Bernard"),
	GRINCH UMETA(DisplayName = "Grinch"),
	MAX UMETA(DisplayName = "Max")
};

UCLASS()
class ADVANCEWARS_2_API ATile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ATile();

	void SetStatus(const ETileStatus _status);

	ETileStatus GetStatus() const;

	void SetPosition(const double InX, const double InY);
	
	FVector2D GetPosition() const;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* Scene;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	ETileStatus Status;

	// (x, y) position of the tile
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	FVector2D TileGridPosition;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
