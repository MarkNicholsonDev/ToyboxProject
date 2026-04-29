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

	AToyboxCharacter* ToyboxCharacter = PlayerController->GetPawn<AToyboxCharacter>();
	if (ToyboxCharacter == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Character fetched"), __FUNCTION__);
		return;
	}

	UE_LOG(LogToybox, Verbose, TEXT("%hs - Granting game context: %s"), __FUNCTION__, *GameContext->GetName());
	AbilitySystemComponent->AddLooseGameplayTag(GameContext->GameContextTag);

	FActiveGameContextAbilityHandles ActiveGameContextHandles;
	ActiveGameContextHandles.Context = GameContext;
	for (FGameContextAbilityBinding AbilityBinding : GameContext->AbilityBindings)
	{
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityBinding.AbilityClass));
		ActiveGameContextHandles.GrantedAbilityHandles.Add(Handle);
		UE_LOG(LogToybox, Verbose, TEXT("%hs - Granted ability: %s"), __FUNCTION__, *AbilityBinding.AbilityClass->GetName());
	}

	ToyboxCharacter->Client_AddGameContextInputBindings(GameContext, ToyboxPlayerState);
	ToyboxCharacter->AddActiveGameContextAbilityHandles(ActiveGameContextHandles);
	ToyboxPlayerState->AddActiveGameContext(GameContext);
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

	if (!AbilitySystemComponent->HasMatchingGameplayTag(GameContext->GameContextTag))
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Attempted to remove GameContext that hasn't been added"), __FUNCTION__);
		return;
	}

	AToyboxCharacter* ToyboxCharacter = PlayerController->GetPawn<AToyboxCharacter>();
	if (ToyboxCharacter == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid Character fetched"), __FUNCTION__);
		return;
	}

	UE_LOG(LogToybox, Verbose, TEXT("%hs - Removing game context: %s"), __FUNCTION__, *GameContext->GetName());
	AbilitySystemComponent->RemoveLooseGameplayTag(GameContext->GameContextTag);

	FActiveGameContextAbilityHandles ActiveGameContextHandles;

	if (!ToyboxCharacter->GetActiveGameContextAbilityHandles(GameContext, ActiveGameContextHandles))
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Cannot find the active game context for game context: %s"), __FUNCTION__, *GameContext->MappingContext->GetName());
		return;
	}

	for (FGameplayAbilitySpecHandle AbilityHandle : ActiveGameContextHandles.GrantedAbilityHandles)
	{
		AbilitySystemComponent->ClearAbility(AbilityHandle);
		UE_LOG(LogToybox, Verbose, TEXT("%hs - Cleared ability: %s"), __FUNCTION__, *AbilityHandle.ToString());
	}

	ToyboxCharacter->Client_RemoveGameContextInputBindings(GameContext);
	ToyboxCharacter->RemoveActiveGameContextAbilityHandles(GameContext);
	ToyboxPlayerState->RemoveActiveGameContext(GameContext);
}	