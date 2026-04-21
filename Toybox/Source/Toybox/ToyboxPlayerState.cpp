// Fill out your copyright notice in the Description page of Project Settings.

#include "ToyboxPlayerState.h"
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

void AToyboxPlayerState::AddActiveGameContext(FActiveGameContext ActiveGameContext)
{
	ActiveGameContexts.Add(ActiveGameContext);
}

FActiveGameContext AToyboxPlayerState::GetActiveGameContext(const UGameContext* GameContext)
{
	for (FActiveGameContext ActiveGameContext : ActiveGameContexts)
	{
		if (ActiveGameContext.Context == GameContext)
		{
			return ActiveGameContext;
		}
	}

	UE_LOG(LogToybox, Warning, TEXT("%hs - Game context not found in the active game contexts, mustn't have been added"), __FUNCTION__);
	return FActiveGameContext();
}

void AToyboxPlayerState::RemoveActiveGameContext(const UGameContext* GameContext)
{
	ActiveGameContexts.RemoveAll([GameContext](const FActiveGameContext& ActiveGameContext)
	{
		return ActiveGameContext.Context == GameContext;
	});
}

void AToyboxPlayerState::OnRep_ActiveGameContexts(const TArray<FActiveGameContext>& OldContexts)
{

}

void AToyboxPlayerState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AToyboxPlayerState, ActiveGameContexts);
}
