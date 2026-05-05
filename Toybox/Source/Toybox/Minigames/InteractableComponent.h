// Copyright 2026 Mark Nicholson. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "Delegates/DelegateCombinations.h"
#include "Components/ActorComponent.h"
#include "InteractableComponent.generated.h"

DECLARE_LOG_CATEGORY_EXTERN(LogInteraction, Verbose, All);

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractionEvent);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class TOYBOX_API UInteractableComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Delegate events
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionEvent OnInteractionBegin;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionEvent OnInteractionCancelled;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractionEvent OnInteractionEnd;

	AActor* InteractingActor;

	// Sets default values for this component's properties
	UInteractableComponent();

	void AttachInteractionArea(USceneComponent* AttachTo = nullptr);

	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	USphereComponent* InteractionArea;

	UPROPERTY(EditDefaultsOnly, Category = "Interaction")
	bool bInteractOnOverlap = false;
	UPROPERTY()
	bool bIsInteractionOngoing = false;

	UFUNCTION()
	void OnInteractionAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult);

	UFUNCTION()
	void OnInteractionAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
		UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UFUNCTION()
	void BeginInteraction();

	UFUNCTION()
	void CancelInteraction();

	UFUNCTION()
	void EndInteraction();
	
};
