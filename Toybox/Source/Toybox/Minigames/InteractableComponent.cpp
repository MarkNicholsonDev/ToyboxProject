// Fill out your copyright notice in the Description page of Project Settings.


#include "Minigames/InteractableComponent.h"
#include "ToyboxCharacter.h"
#include "ToyboxPlayerController.h"

DEFINE_LOG_CATEGORY(LogInteraction);

// Sets default values for this component's properties
UInteractableComponent::UInteractableComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// Create and attach a sphere component
	InteractionArea = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionArea"));
}

void UInteractableComponent::AttachInteractionArea(USceneComponent* AttachTo)
{
	if (AttachTo == nullptr)
	{
		AActor* Owner = GetOwner();
		if (Owner == nullptr)
		{
			UE_LOG(LogInteraction, Error, TEXT("%hs - Cannot attach interaction area to owner, invalid owner"), __FUNCTION__);
			return;
		}
		AttachTo = Owner->GetRootComponent();
	}

	if (InteractionArea == nullptr) 
	{
		UE_LOG(LogInteraction, Error, TEXT("%hs - Cannot attach interaction area to owner, invalid interaction area"), __FUNCTION__);
		return;
	}
	InteractionArea->AttachToComponent(AttachTo, FAttachmentTransformRules::KeepRelativeTransform);
}

// Called when the game starts
void UInteractableComponent::BeginPlay()
{
	Super::BeginPlay();

	InteractionArea->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionAreaBeginOverlap);
	InteractionArea->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionAreaEndOverlap);
	
}

// Called every frame
void UInteractableComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UInteractableComponent::OnInteractionAreaBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepHitResult)
{
	UE_LOG(LogInteraction, Verbose, TEXT("Entered Interaction Area"));

	InteractingActor = OtherActor;

	if (bInteractOnOverlap)
	{
		BeginInteraction();
	}
	else
	{
		// Add the prompt/ability to interact with the player here
		AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(OtherActor);

		if (PlayerCharacter == nullptr)
		{
			UE_LOG(LogInteraction, Error, TEXT("%hs - Invalid player character"), __FUNCTION__);
			return;
		}

		PlayerCharacter->OnInteractDelegate.AddDynamic(this, &UInteractableComponent::BeginInteraction);
	}
}

void UInteractableComponent::OnInteractionAreaEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogInteraction, Verbose, TEXT("Left Interaction Area"));

	if (bInteractOnOverlap)
	{
		EndInteraction();
	}
	else
	{
		// Add the prompt/ability to interact with the player here

		AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(OtherActor);

		if (PlayerCharacter == nullptr)
		{
			UE_LOG(LogInteraction, Error, TEXT("%hs - Invalid player character"), __FUNCTION__);
			return;
		}

		// Should add the remove the exit context stuff here
		PlayerCharacter->OnInteractDelegate.RemoveDynamic(this, &UInteractableComponent::BeginInteraction);

		if (bIsInteractionOngoing)
		{
			EndInteraction();
		}
	}

	InteractingActor = nullptr;
}

void UInteractableComponent::BeginInteraction()
{
	AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(InteractingActor);

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("%hs - Invalid player character"), __FUNCTION__);
		return;
	}

	// Should add the exit context stuff here
	PlayerCharacter->ExitContextDelegate.AddDynamic(this, &UInteractableComponent::EndInteraction);

	if (OnInteractionBegin.IsBound())
	{
		OnInteractionBegin.Broadcast();
	}

	bIsInteractionOngoing = true;
}

void UInteractableComponent::CancelInteraction()
{
	AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(InteractingActor);

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("%hs - Invalid player character"), __FUNCTION__);
		return;
	}

	if (OnInteractionCancelled.IsBound())
	{
		OnInteractionCancelled.Broadcast();
	}

	// Should add the exit context stuff here
	PlayerCharacter->ExitContextDelegate.RemoveDynamic(this, &UInteractableComponent::EndInteraction);

	InteractingActor = nullptr;
	bIsInteractionOngoing = false;
}

void UInteractableComponent::EndInteraction()
{
	AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(InteractingActor);

	if (PlayerCharacter == nullptr)
	{
		UE_LOG(LogInteraction, Error, TEXT("%hs - Invalid player character"), __FUNCTION__);
		return;
	}

	if (OnInteractionEnd.IsBound()) 
	{
		OnInteractionEnd.Broadcast();
	}

	// Should add the exit context stuff here
	PlayerCharacter->ExitContextDelegate.RemoveDynamic(this, &UInteractableComponent::EndInteraction);

	InteractingActor = nullptr;
	bIsInteractionOngoing = false;
}

