// Copyright 2026 Mark Nicholson. All Rights Reserved.


#include "Minigames/MinigameInteractable.h"
#include "Minigame.h"

// Sets default values
AMinigameInteractable::AMinigameInteractable()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRootComponent"));
	RootComponent = SceneRootComponent;

	// Create and attach a interactable component
	InteractableComponent = CreateDefaultSubobject<UInteractableComponent>(TEXT("InteractionArea"));
	InteractableComponent->AttachInteractionArea();

	InteractableMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("InteractableMesh"));
	InteractableMesh->SetupAttachment(SceneRootComponent);
}

// Called when the game starts or when spawned
void AMinigameInteractable::BeginPlay()
{
	Super::BeginPlay();
	
	InteractableComponent->OnInteractionBegin.AddDynamic(this, &AMinigameInteractable::StartMinigame);
	InteractableComponent->OnInteractionCancelled.AddDynamic(this, &AMinigameInteractable::StopMinigame);
	InteractableComponent->OnInteractionEnd.AddDynamic(this, &AMinigameInteractable::StopMinigame);
}

void AMinigameInteractable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	StopMinigame();
}

// Called every frame
void AMinigameInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AMinigameInteractable::StartMinigame()
{
	UE_LOG(LogMinigame, Warning, TEXT("%hs - HasAuthority: %s"), __FUNCTION__, HasAuthority() ? TEXT("true") : TEXT("false"));

	if (MinigameClass == nullptr) 
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - No assigned minigame class to create on starting minigame"), __FUNCTION__);
		return;
	}

	if (Minigame != nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Minigame already running, cannot create another instance"), __FUNCTION__);
		return;
	}

	FActorSpawnParameters SpawnParams;
	SpawnParams.Owner = this;
	Minigame = GetWorld()->SpawnActor<AMinigame>(MinigameClass, GetActorTransform(), SpawnParams);
	if (Minigame == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid Minigame spawned"), __FUNCTION__);
		return;
	}

	if (InteractableComponent == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid InteractableComponent fetched"), __FUNCTION__);
		return;
	}

	APawn* Pawn = Cast<APawn>(InteractableComponent->InteractingActor);
	if (Pawn == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid pawn fetched"), __FUNCTION__);
		return;
	}

	AToyboxPlayerController* PlayerController = Cast<AToyboxPlayerController>(Pawn->GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogMinigame, Error, TEXT("%hs - Invalid PlayerController fetched"), __FUNCTION__);
		return;
	}

	Minigame->OwningInteractable = this;
	Minigame->OnMinigameEndedDelegate.AddDynamic(this, &ThisClass::CleanupMinigame);
	Minigame->StartMinigame(PlayerController);

	UE_LOG(LogMinigame, Verbose, TEXT("%hs - Minigame started: %s"), __FUNCTION__, *MinigameClass->GetName());
}

void AMinigameInteractable::StopMinigame()
{
	if (Minigame == nullptr)
	{
		return;
	}

	if (this->HasAuthority())
	{
		Minigame->EndMinigame();
	}
}

void AMinigameInteractable::CleanupMinigame()
{
	if (Minigame == nullptr)
	{
		return;
	}

	if (this->HasAuthority())
	{
		UE_LOG(LogMinigame, Verbose, TEXT("%hs - Destroying minigame: %s"), __FUNCTION__, *Minigame->GetName());

		Minigame->OnMinigameEndedDelegate.Clear();
		Minigame->Destroy();
		Minigame = nullptr;
	}
}