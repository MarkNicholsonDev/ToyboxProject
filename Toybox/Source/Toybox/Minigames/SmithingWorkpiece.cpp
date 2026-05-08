// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingWorkpiece.h"

// Sets default values
ASmithingWorkpiece::ASmithingWorkpiece()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASmithingWorkpiece::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmithingWorkpiece::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmithingWorkpiece::ApplyHeat(const float HeatValue)
{
	if (SmithingData.CurrentHeat <= HeatValue)
	{
		SmithingData.CurrentHeat = HeatValue;
		UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Current Workpiece Heat: %s"), __FUNCTION__, *FString::SanitizeFloat(SmithingData.CurrentHeat));
	}
}

