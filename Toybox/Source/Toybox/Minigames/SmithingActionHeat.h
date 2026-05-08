// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/SmithingAction.h"
#include "SmithingActionHeat.generated.h"

USTRUCT()
struct FHeatingChargeData 
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY()
	float CurrentCharge = 0.0f;

	UPROPERTY()
	float MinCharge = 0.0f;

	UPROPERTY()
	float MaxCharge = 1.0f;

	UPROPERTY(EditDefaultsOnly)
	float ChargeRate = 0.1f;

	UPROPERTY(EditDefaultsOnly)
	float DrainRate = 0.25f;

	UPROPERTY(EditDefaultsOnly)
	float InteractThreshold = 0.2f;

	UPROPERTY()
	bool bIsDraining = false;

	UPROPERTY()
	bool bCanInteract = false;
};

/**
 * 
 */
UCLASS()
class TOYBOX_API ASmithingActionHeat : public ASmithingAction
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASmithingActionHeat();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PerformAction() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditDefaultsOnly)
	FHeatingChargeData ChargingData;
	
	void IncreaseCharge(const float DeltaTime);
	void DrainCharge(const float DeltaTime);

	void ApplyChargeToWorkpiece();
};
