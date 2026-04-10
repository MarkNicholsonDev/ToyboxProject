// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "InputAction.h"
#include "Abilities/GameplayAbility.h"
#include "GameplayTagContainer.h"
#include "GameContext.generated.h"

/**
 * Used to grant a set of gameplay abilities and an IMC to the player with a gameplay tag to track it on the server
 */
UCLASS()
class TOYBOX_API UGameContext : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TMap<UInputAction*, TSubclassOf<UGameplayAbility>> AbilityBindings;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	FGameplayTag GameContextTag;

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	UInputMappingContext* MappingContext;
};


/**
 * Granted to the player character to track granted game contexts
 */
USTRUCT()
struct FActiveGameContext
{
	GENERATED_BODY()

	FActiveGameContext() : Context(nullptr) {}

	UPROPERTY()
	const UGameContext* Context;

	UPROPERTY()
	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;

	// We store the input bindings as uint32s because FEnhancedInputActionEventBinding is not a USTRUCT so it can't be stored in a TArray in the ActiveGameContext, we store the handle of the event binding instead.
	UPROPERTY()
	TArray<uint32> GrantedActionHandles;
};
