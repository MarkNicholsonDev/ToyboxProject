// Copyright 2026 Mark Nicholson. All Rights Reserved.

#include "DebugGameplayAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Toybox.h"
#include "ExitContextAbility.h"

UDebugGameplayAbility::UDebugGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDebugGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	CommitAbility(Handle, ActorInfo, ActivationInfo);

	UE_LOG(LogToybox, Verbose, TEXT("%hs - Activated Ability - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));

	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDebugGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogToybox, Verbose, TEXT("%hs - Ability Ended - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));
}
