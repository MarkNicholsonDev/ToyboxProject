// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/Minigame.h"
#include "FishingMinigame.generated.h"

UENUM()
enum class EFishingMinigameState : uint8
{
	Idle,
	Starting,
	Active,
	Completed,
	Cleanup,
	CleanupComplete,
};

UCLASS()
class TOYBOX_API AFishingMinigame : public AMinigame
{
	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	AFishingMinigame();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartMinigame(AToyboxPlayerController* PC) override;
	void EndMinigame() override;

private:
	bool bCompletedMinigame = false;

	EFishingMinigameState MinigameState = EFishingMinigameState::Idle;

	void SwitchMinigameState(EFishingMinigameState NewState);
};
