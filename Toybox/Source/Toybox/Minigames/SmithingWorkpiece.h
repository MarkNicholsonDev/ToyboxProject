// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithingWorkpiece.generated.h"

USTRUCT()
struct FSmithingData
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float CurrentHeat = 0.0f;

	UPROPERTY()
	float CurrentHammeringProgress = 0.0f;
};

UCLASS()
class TOYBOX_API ASmithingWorkpiece : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmithingWorkpiece();

	void ApplyHeat(const float HeatValue);

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY()
	FSmithingData SmithingData;
};
