// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SmithingAction.generated.h"

class ASmithingWorkpiece;

UCLASS()
class TOYBOX_API ASmithingAction : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASmithingAction();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	virtual void PerformAction();

	void SetSmithingWorkpiece(ASmithingWorkpiece* Workpiece);

protected:
	bool bActionCompleted = false;

	UPROPERTY(Replicated)
	ASmithingWorkpiece* SmithingWorkpiece;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
