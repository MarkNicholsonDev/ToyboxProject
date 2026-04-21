// Fill out your copyright notice in the Description page of Project Settings.

#include "GameContextLibrary.h"
#include "GameContext.h"
#include "ToyboxPlayerController.h"
#include "ToyboxCharacter.h"
#include "ToyboxPlayerState.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Toybox.h"

void UGameContextLibrary::ApplyGameContext(const UGameContext* GameContext, AToyboxPlayerController* PlayerController)
{
	if (GameContext == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid GameContext param"), __FUNCTION__);
		return;
	}

	if (PlayerController == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerController param"), __FUNCTION__);
		return;
	}

	if (!PlayerController->HasAuthority())
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Can only add a game context on the server"), __FUNCTION__);
		return;
	}

	AToyboxPlayerState* ToyboxPlayerState = PlayerController->GetPlayerState<AToyboxPlayerState>();
	if (ToyboxPlayerState == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerState cast"), __FUNCTION__);
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ToyboxPlayerState->GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid AbilitySystemComponent fetched"), __FUNCTION__);
		return;
	}

	if (AbilitySystemComponent->HasMatchingGameplayTag(GameContext->GameContextTag)) 
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Attempted to add GameContext that is already added"), __FUNCTION__);
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid LocalPlayer fetched"), __FUNCTION__);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Subsystem == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Subsystem fetched"), __FUNCTION__);
		return;
	}

	AToyboxCharacter* ToyboxCharacter = PlayerController->GetPawn<AToyboxCharacter>();
	if (ToyboxCharacter == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Character fetched"), __FUNCTION__);
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(ToyboxCharacter->InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid EnhancedInputComponent fetched"), __FUNCTION__);
		return;
	}

	AbilitySystemComponent->AddLooseGameplayTag(GameContext->GameContextTag);

	for (FGameContextAbilityBinding AbilityBinding : GameContext->AbilityBindings)
	{
		AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityBinding.AbilityClass));
		UE_LOG(LogToybox, Display, TEXT("%hs - Granted ability: %s"), __FUNCTION__, *AbilityBinding.AbilityClass->GetName());
	}

	ToyboxCharacter->Client_AddGameContextInputBindings(GameContext, PlayerController);
}

void UGameContextLibrary::RemoveGameContext(const UGameContext* GameContext, AToyboxPlayerController* PlayerController)
{
	if (GameContext == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid GameContext param"), __FUNCTION__);
		return;
	}

	if (PlayerController == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerController param"), __FUNCTION__);
		return;
	}

	if (!PlayerController->HasAuthority())
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Can only remove a game context on the server"), __FUNCTION__);
		return;
	}

	AToyboxPlayerState* ToyboxPlayerState = PlayerController->GetPlayerState<AToyboxPlayerState>();
	if (ToyboxPlayerState == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerState cast"), __FUNCTION__);
		return;
	}

	UAbilitySystemComponent* AbilitySystemComponent = ToyboxPlayerState->GetAbilitySystemComponent();
	if (AbilitySystemComponent == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid AbilitySystemComponent fetched"), __FUNCTION__);
		return;
	}

	if (!AbilitySystemComponent->HasMatchingGameplayTag(GameContext->GameContextTag))
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Attempted to add GameContext that hasn't been added to the character"), __FUNCTION__);
		return;
	}

	ULocalPlayer* LocalPlayer = PlayerController->GetLocalPlayer();
	if (LocalPlayer == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid LocalPlayer fetched"), __FUNCTION__);
		return;
	}

	UEnhancedInputLocalPlayerSubsystem* Subsystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>();
	if (Subsystem == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Subsystem fetched"), __FUNCTION__);
		return;
	}

	AToyboxCharacter* ToyboxCharacter = PlayerController->GetPawn<AToyboxCharacter>();
	if (ToyboxCharacter == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Character fetched"), __FUNCTION__);
		return;
	}

	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(ToyboxCharacter->InputComponent);
	if (EnhancedInputComponent == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid EnhancedInputComponent fetched"), __FUNCTION__);
		return;
	}

	// Add at priority 1 so any context added IMCs are above the default mapping context
	Subsystem->RemoveMappingContext(GameContext->MappingContext);
	AbilitySystemComponent->RemoveLooseGameplayTag(GameContext->GameContextTag);

	FActiveGameContext ActiveGameContext = ToyboxPlayerState->GetActiveGameContext(GameContext);

	for (uint32 ActionHandle : ActiveGameContext.GrantedActionHandles)
	{
		EnhancedInputComponent->RemoveBindingByHandle(ActionHandle);
	}

	/*for (FGameplayAbilitySpecHandle ActiveHandle : ActiveGameContext.GrantedAbilityHandles)
	{
		AbilitySystemComponent->ClearAbility(ActiveHandle);
	}*/

	ToyboxPlayerState->RemoveActiveGameContext(GameContext);
}