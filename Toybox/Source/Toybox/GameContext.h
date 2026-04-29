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
 * Stored by the game context to store everything needed for one ability binding
 */
USTRUCT(BlueprintType)
struct FGameContextAbilityBinding
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UGameplayAbility> AbilityClass;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InputAction;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag AbilityTag;
};

/**
 * Used to grant a set of gameplay abilities and an IMC to the player with a gameplay tag to track it on the server
 */
UCLASS()
class TOYBOX_API UGameContext : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	TArray<FGameContextAbilityBinding> AbilityBindings;

	UPROPERTY(EditDefaultsOnly)
	FGameplayTag GameContextTag;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* MappingContext;
};

/**
 * Granted to the player character to track granted game contexts
 */
USTRUCT()
struct FActiveGameContextActionHandles
{
	GENERATED_BODY()

	FActiveGameContextActionHandles() : Context(nullptr) {}

	UPROPERTY()
	const UGameContext* Context;

	// We store the input bindings as uint32s because FEnhancedInputActionEventBinding is not a USTRUCT
	// so it can't be stored in a TArray in the ActiveGameContext, we store the int representation of the event binding handle instead.
	TArray<uint32> GrantedActionHandles;
};

/**
 * Granted to the player character to store the ability handles for the granted game context
 */
USTRUCT()
struct FActiveGameContextAbilityHandles
{
	GENERATED_BODY()

	FActiveGameContextAbilityHandles() : Context(nullptr) {}

	UPROPERTY()
	const UGameContext* Context;

	TArray<FGameplayAbilitySpecHandle> GrantedAbilityHandles;
};