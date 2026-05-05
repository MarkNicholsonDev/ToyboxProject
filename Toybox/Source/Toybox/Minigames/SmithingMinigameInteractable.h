// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/MinigameInteractable.h"
#include "SmithingMinigameInteractable.generated.h"

/**
 * 
 */
UCLASS()
class TOYBOX_API ASmithingMinigameInteractable : public AMinigameInteractable
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASmithingMinigameInteractable();

private:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* ForgeMesh;
};
