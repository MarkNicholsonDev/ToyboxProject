// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableComponent.h"
#include "MinigameInteractable.generated.h"

// Forward declare instead of include
class AMinigame;

UCLASS()
class TOYBOX_API AMinigameInteractable : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AMinigameInteractable();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UFUNCTION()
	void StartMinigame();

	UFUNCTION()
	void StopMinigame();

	UFUNCTION()
	void CleanupMinigame();

private:
	UPROPERTY(EditDefaultsOnly)
	USceneComponent* SceneRootComponent;

	UPROPERTY(EditDefaultsOnly)
	UInteractableComponent* InteractableComponent;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* InteractableMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Minigame")
	TSubclassOf<AMinigame> MinigameClass;

	UPROPERTY()
	AMinigame* Minigame;
};
