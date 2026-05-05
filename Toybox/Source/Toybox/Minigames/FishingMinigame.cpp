// Copyright 2026 Mark Nicholson. All Rights Reserved.


#include "Minigames/FishingMinigame.h"

// Sets default values
AFishingMinigame::AFishingMinigame()
{

}

// Called when the game starts or when spawned
void AFishingMinigame::BeginPlay()
{
	Super::BeginPlay();

	UE_LOG(LogMinigame, Verbose, TEXT("%hs - Minigame is alive!"), __FUNCTION__);
}

void AFishingMinigame::StartMinigame(AToyboxPlayerController* PC)
{
	Super::StartMinigame(PC);

	UE_LOG(LogMinigame, Verbose, TEXT("%hs - Starting Minigame!"), __FUNCTION__);
	SwitchMinigameState(EFishingMinigameState::Starting);
}

void AFishingMinigame::EndMinigame()
{
	Super::EndMinigame();

	UE_LOG(LogMinigame, Verbose, TEXT("%hs - Ending Minigame!"), __FUNCTION__);
	SwitchMinigameState(EFishingMinigameState::Cleanup);
}

// Called every frame
void AFishingMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (MinigameState)
	{
	case EFishingMinigameState::Idle:
		
		break;
	case EFishingMinigameState::Starting:
		SwitchMinigameState(EFishingMinigameState::Active);
		break;
	case EFishingMinigameState::Active:
		if (bCompletedMinigame)
		{
			SwitchMinigameState(EFishingMinigameState::Completed);
		}
		break;
	case EFishingMinigameState::Completed:
		SwitchMinigameState(EFishingMinigameState::Cleanup);
		break;
	case EFishingMinigameState::Cleanup:
		Cleanup();
		break;
	case EFishingMinigameState::CleanupComplete:
		break;
	default:
		break;
	}
}

void AFishingMinigame::SwitchMinigameState(EFishingMinigameState NewState)
{
	UE_LOG(LogMinigame, Verbose, TEXT("%hs - Minigame state switching from %s to %s"), __FUNCTION__, *UEnum::GetValueAsString(MinigameState), *UEnum::GetValueAsString(NewState));
	MinigameState = NewState;
}

void AFishingMinigame::Cleanup()
{
	OnMinigameEndedDelegate.Broadcast();
	//OnMinigameEndedDelegate.Clear();
}