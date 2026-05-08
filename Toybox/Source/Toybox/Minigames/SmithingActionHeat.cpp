// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingActionHeat.h"
#include "Minigames/SmithingWorkpiece.h"

// Sets default values
ASmithingActionHeat::ASmithingActionHeat()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ASmithingActionHeat::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void ASmithingActionHeat::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Current Charge: %s"), __FUNCTION__, *FString::SanitizeFloat(ChargingData.CurrentCharge));

	if (!ChargingData.bIsDraining) 
	{
		IncreaseCharge(DeltaTime);
	}
	else 
	{
		DrainCharge(DeltaTime);
	}
}

void ASmithingActionHeat::IncreaseCharge(const float DeltaTime)
{
	ChargingData.CurrentCharge = FMath::Clamp(ChargingData.CurrentCharge + (ChargingData.ChargeRate * DeltaTime), ChargingData.MinCharge, ChargingData.MaxCharge);

	if (ChargingData.CurrentCharge >= ChargingData.InteractThreshold)
	{
		ChargingData.bCanInteract = true;
	}
}

void ASmithingActionHeat::DrainCharge(const float DeltaTime)
{
	ChargingData.CurrentCharge = FMath::Clamp(ChargingData.CurrentCharge - (ChargingData.DrainRate * DeltaTime), ChargingData.MinCharge, ChargingData.MaxCharge);

	if (ChargingData.bCanInteract)
	{
		ChargingData.bCanInteract = false;
	}

	if (ChargingData.CurrentCharge <= ChargingData.MinCharge)
	{
		ChargingData.bIsDraining = false;
	}
}


void ASmithingActionHeat::PerformAction()
{
	ChargingData.bIsDraining = true;

	ApplyChargeToWorkpiece();
}

void ASmithingActionHeat::ApplyChargeToWorkpiece()
{
	// Insufficent charge blocks the ability to apply charge
	if (!ChargingData.bCanInteract) 
	{
		return;
	}

	if (SmithingWorkpiece)
	{
		SmithingWorkpiece->ApplyHeat(ChargingData.CurrentCharge);
	}
}

