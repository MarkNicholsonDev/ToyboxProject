// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "Minigames/SmithingInteractAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "SmithingAction.h"

USmithingInteractAbility::USmithingInteractAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void USmithingInteractAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Activated Ability - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));

	// Note: Will be switched to fetching the interacted object from the player instead rather than GetActorOfClass as it's more costly - Fine for a test
	AActor* FoundActor = UGameplayStatics::GetActorOfClass(GetWorld(), ASmithingAction::StaticClass());
	if (FoundActor == nullptr)
	{
		UE_LOG(LogSmithingMinigame, Error, TEXT("%hs - No smithing action could be found"), __FUNCTION__);
		return;
	}

	ASmithingAction* SmithingAction = Cast<ASmithingAction>(FoundActor);
	if (SmithingAction == nullptr)
	{
		UE_LOG(LogSmithingMinigame, Error, TEXT("%hs - Invalid SmithingAction cast"), __FUNCTION__);
		return;
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);

	SmithingAction->PerformAction();

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void USmithingInteractAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogSmithingMinigame, Verbose, TEXT("%hs - Ability Ended - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));
}
