// Copyright 2026 Mark Nicholson. All Rights Reserved.


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
	bReplicates = true;
}

// Called when the game starts or when spawned
void AMinigame::BeginPlay()
{
	Super::BeginPlay();
	
}

void AMinigame::StartMinigame(AToyboxPlayerController* PC)
{
	UE_LOG(LogMinigame, Warning, TEXT("%hs - HasAuthority: %s"), __FUNCTION__, HasAuthority() ? TEXT("true") : TEXT("false"));

	if (PC == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid PC provided to minigame, cannot start minigame"), __FUNCTION__);
		return;
	}

	PlayerController = PC;

	if (HasAuthority()) 
	{
		UGameContextLibrary::ApplyGameContext(MinigameContext, PlayerController);
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

void AMinigame::EndMinigame() 
{

}

void AMinigame::Cleanup()
{
	if (PlayerController != nullptr)
	{
		if (HasAuthority())
		{
			UGameContextLibrary::RemoveGameContext(MinigameContext, PlayerController);
		}
	}
	else
	{
		UE_LOG(LogMinigame, Warning, TEXT("%hs - Invalid PlayerController, cannot remove game context from player if it still exists"), __FUNCTION__);
	}

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called every frame
void AMinigame::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

