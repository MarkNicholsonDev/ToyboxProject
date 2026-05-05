// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingMinigameInteractable.h"

ASmithingMinigameInteractable::ASmithingMinigameInteractable() : Super()
{
	ForgeMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ForgeMesh"));
	ForgeMesh->SetupAttachment(SceneRootComponent);
}
