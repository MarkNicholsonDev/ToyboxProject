// Fill out your copyright notice in the Description page of Project Settings.

#include "InteractAbility.h"
#include "ToyboxCharacter.h"
#include "Toybox.h"

UInteractAbility::UInteractAbility()
{
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
}

void UInteractAbility::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	UE_LOG(LogToybox, Warning, TEXT("%hs - Activated Ability - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));

	if (ActorInfo->AvatarActor->HasAuthority())
	{
		AToyboxCharacter* PlayerCharacter = Cast<AToyboxCharacter>(ActorInfo->AvatarActor);
		if (PlayerCharacter == nullptr)
		{
			UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerCharacter cast"), __FUNCTION__);
			return;
		}

		const FInputActionValue Value;
		PlayerCharacter->StartInteraction(Value);
	}

	CommitAbility(Handle, ActorInfo, ActivationInfo);
	EndAbility(Handle, ActorInfo, ActivationInfo, true, false);
}

void UInteractAbility::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	UE_LOG(LogToybox, Warning, TEXT("%hs - Ability Ended - %s"), __FUNCTION__, HasAuthority(&CurrentActivationInfo) ? TEXT("Server") : TEXT("Client"));
}