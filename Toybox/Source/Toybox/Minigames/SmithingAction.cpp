// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingAction.h"
#include "Minigames/SmithingWorkpiece.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ASmithingAction::ASmithingAction()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	bReplicates = true;
}

// Called when the game starts or when spawned
void ASmithingAction::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASmithingAction::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASmithingAction::PerformAction()
{
	
}

void ASmithingAction::SetSmithingWorkpiece(ASmithingWorkpiece* Workpiece)
{
	if (Workpiece == nullptr)
	{
		UE_LOG(LogSmithingMinigame, Error, TEXT("%hs - Invalid SmithingWorkpiece param"), __FUNCTION__);
		return;
	}

	SmithingWorkpiece = Workpiece;
}

void ASmithingAction::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASmithingAction, SmithingWorkpiece);
}

