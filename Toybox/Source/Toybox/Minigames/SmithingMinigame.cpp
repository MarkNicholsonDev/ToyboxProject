// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingMinigame.h"
#include "Minigames/SmithingAction.h"
#include "Net/UnrealNetwork.h"

DEFINE_LOG_CATEGORY(LogSmithingMinigame);

// Sets default values
ASmithingMinigame::ASmithingMinigame() : Super()
{
	
}

// Called when the game starts or when spawned
void ASmithingMinigame::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Minigame is alive!"), __FUNCTION__);
}

void ASmithingMinigame::StartMinigame(AToyboxPlayerController* PC)
{
	Super::StartMinigame(PC);

	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Starting Minigame!"), __FUNCTION__);
	SwitchMinigameState(ESmithingMinigameState::Starting);
}

void ASmithingMinigame::EndMinigame()
{
	Super::EndMinigame();

	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Ending Minigame!"), __FUNCTION__);
	SwitchMinigameState(ESmithingMinigameState::Cleanup);
}

// Called every frame
void ASmithingMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MinigameState)
	{
	case ESmithingMinigameState::Idle:

		break;
	case ESmithingMinigameState::Starting:
		SwitchMinigameState(ESmithingMinigameState::Active);
		break;
	case ESmithingMinigameState::Active:
		if (SmithingWorkpiece == nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SmithingWorkpiece = GetWorld()->SpawnActor<ASmithingWorkpiece>(SmithingWorkpieceClass, GetActorTransform(), SpawnParams);
			if (SmithingWorkpiece == nullptr)
			{
				UE_LOG(LogSmithingMinigame, Error, TEXT("%hs - Invalid SmithingWorkpiece spawned"), __FUNCTION__);
				return;
			}
		}

		if (SmithingAction == nullptr)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SmithingAction = GetWorld()->SpawnActor<ASmithingAction>(SmithingActionClass, GetActorTransform(), SpawnParams);
			if (SmithingAction == nullptr)
			{
				UE_LOG(LogSmithingMinigame, Error, TEXT("%hs - Invalid SmithingAction spawned"), __FUNCTION__);
				return;
			}

			SmithingAction->SetSmithingWorkpiece(SmithingWorkpiece);
		}

		break;
	case ESmithingMinigameState::Completed:
		SwitchMinigameState(ESmithingMinigameState::Cleanup);
		break;
	case ESmithingMinigameState::Cleanup:
		Cleanup();
		break;
	case ESmithingMinigameState::CleanupComplete:
		break;
	default:
		break;
	}
}

void ASmithingMinigame::SwitchMinigameState(ESmithingMinigameState NewState)
{
	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Minigame state switching from %s to %s"), __FUNCTION__, *UEnum::GetValueAsString(MinigameState), *UEnum::GetValueAsString(NewState));
	MinigameState = NewState;
}

void ASmithingMinigame::Cleanup()
{
	Super::Cleanup();

	SwitchMinigameState(ESmithingMinigameState::CleanupComplete);

	// Trigger minigame destruction
	OnMinigameEndedDelegate.Broadcast();
	OnMinigameEndedDelegate.Clear();
}

void ASmithingMinigame::CompletedMinigame()
{
	bCompletedMinigame = true;
}

void ASmithingMinigame::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ASmithingMinigame, SmithingAction);
	DOREPLIFETIME(ASmithingMinigame, SmithingWorkpiece);
}