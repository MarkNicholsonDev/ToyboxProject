// Fill out your copyright notice in the Description page of Project Settings.

#include "DebugGameplayAbility.h"
#include "Abilities/GameplayAbilityTargetTypes.h"
#include "Toybox.h"

UDebugGameplayAbility::UDebugGameplayAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UDebugGameplayAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogToybox, Warning, TEXT("%hs - Activated Ability - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UDebugGameplayAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogToybox, Warning, TEXT("%hs - Ability Ended - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));
}
