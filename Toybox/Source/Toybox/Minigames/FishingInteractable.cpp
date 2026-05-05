// Copyright 2026 Mark Nicholson. All Rights Reserved.


#include "Minigames/FishingInteractable.h"

// Sets default values
AFishingInteractable::AFishingInteractable() : Super()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void AFishingInteractable::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFishingInteractable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFishingInteractable::StartMinigame()
{
	Super::StartMinigame();

}