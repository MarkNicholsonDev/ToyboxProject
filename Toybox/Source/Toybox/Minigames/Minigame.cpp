// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/Minigame.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "GameContextLibrary.h"

DEFINE_LOG_CATEGORY(LogMinigame);

// Sets default values
AMinigame::AMinigame()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AMinigame::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMinigame::StartMinigame(AToyboxPlayerController* PC)
{
	if (PC == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid PC provided to minigame, cannot start minigame"), __FUNCTION__);
		return;
	}

	PlayerController = PC;
	AToyboxCharacter* Pawn = PlayerController->GetPawn<AToyboxCharacter>();
	if (Pawn == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid pawn fetched from PC, cannot start minigame"), __FUNCTION__);
		return;
	}

	PlayerCharacter = Pawn;
	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid Player fetched from PC, can't add the minigame mapping context"), __FUNCTION__);
		return;
	}

	UGameContextLibrary::ApplyGameContext(MinigameContext, PlayerController);

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMinigame::EndMinigame()
{
	ULocalPlayer* Player = PlayerController->GetLocalPlayer();
	if (Player == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid Player fetched from PC, can't remove the minigame mapping context"), __FUNCTION__);
		return;
	}

	UGameContextLibrary::RemoveGameContext(MinigameContext, PlayerController);
}

void AMinigame::Cleanup()
{
	
}

// Called every frame
void AMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

