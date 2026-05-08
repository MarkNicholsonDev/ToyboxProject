// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Minigames/Minigame.h"
#include "SmithingMinigame.generated.h"

class ASmithingAction;
class ASmithingWorkpiece;

DECLARE_LOG_CATEGORY_EXTERN(LogSmithingMinigame, Verbose, All);

UENUM()
enum class ESmithingMinigameState : uint8
{
	Idle,
	Starting,
	Active,
	Completed,
	Cleanup,
	CleanupComplete,
};

UCLASS()
class TOYBOX_API ASmithingMinigame : public AMinigame
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASmithingMinigame();

	// To be removed - just for testing minigame flow
	void CompletedMinigame();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void StartMinigame(AToyboxPlayerController* PC) override;

	void EndMinigame() override;

	void Cleanup() override;

private:
	UPROPERTY()
	bool bCompletedMinigame = false;

	UPROPERTY()
	ESmithingMinigameState MinigameState = ESmithingMinigameState::Idle;

	UPROPERTY(Replicated)
	ASmithingAction* SmithingAction;

	UPROPERTY(Replicated)
	ASmithingWorkpiece* SmithingWorkpiece;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASmithingAction> SmithingActionClass;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ASmithingWorkpiece> SmithingWorkpieceClass;

	void SwitchMinigameState(ESmithingMinigameState NewState);

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
