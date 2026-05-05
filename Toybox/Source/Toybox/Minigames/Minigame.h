// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ToyboxCharacter.h"
#include "ToyboxPlayerController.h"
#include "Minigame.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogMinigame, Verbose, All);

// Forward declare instead of include
class AMinigameInteractable;
class UGameContext;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMinigameEndedDelegate);

UCLASS()
class TOYBOX_API AMinigame : public AActor
{
	GENERATED_BODY()
	
public:	
	UPROPERTY()
	AMinigameInteractable* OwningInteractable;

	UPROPERTY()
	FOnMinigameEndedDelegate OnMinigameEndedDelegate;

	// Sets default values for this actor's properties
	AMinigame();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void StartMinigame(AToyboxPlayerController* PC);

	virtual void EndMinigame();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Cleanup();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Minigame")
	UGameContext* MinigameContext;

	AToyboxPlayerController* PlayerController;
	AToyboxCharacter* PlayerCharacter;
};
