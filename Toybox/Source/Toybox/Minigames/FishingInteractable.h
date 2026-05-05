// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/MinigameInteractable.h"
#include "FishingInteractable.generated.h"

/**
 * 
 */
UCLASS()
class TOYBOX_API AFishingInteractable : public AMinigameInteractable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFishingInteractable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


private:
	void StartMinigame();
};
