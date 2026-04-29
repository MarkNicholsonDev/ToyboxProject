// Fill out your copyright notice in the Description page of Project Settings.

#include "ToyboxPlayerState.h"
#include "ToyboxCharacter.h"
#include "Toybox.h"
#include "Net/UnrealNetwork.h"

AToyboxPlayerState::AToyboxPlayerState()
{
	ASC = CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	ASC->SetIsReplicated(true);
	ASC->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);
}

UAbilitySystemComponent* AToyboxPlayerState::GetAbilitySystemComponent() const
{
	return ASC;
}

void AToyboxPlayerState::AddActiveGameContext(const UGameContext* GameContext)
{
	ActiveGameContexts.Add(GameContext);
}

void AToyboxPlayerState::RemoveActiveGameContext(const UGameContext* GameContext)
{
	ActiveGameContexts.RemoveAll([GameContext](const UGameContext* ActiveGameContext)
		{
			return ActiveGameContext == GameContext;
		});
}

TArray<const UGameContext*> AToyboxPlayerState::GetActiveGameContexts()
{
	return ActiveGameContexts;
}

void AToyboxPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToyboxPlayerState, ActiveGameContexts);
}
