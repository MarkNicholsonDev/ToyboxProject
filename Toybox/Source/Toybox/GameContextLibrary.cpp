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

	// Add at priority 1 so any context added IMCs are above the default mapping context
	Subsystem->AddMappingContext(GameContext->MappingContext, 1);
	AbilitySystemComponent->AddLooseGameplayTag(GameContext->GameContextTag);

	TArray<FGameplayAbilitySpecHandle> ActiveAbilityHandles;

	// We store the input bindings as uint32s because FEnhancedInputActionEventBinding is not a USTRUCT so it can't be stored in a TArray in the ActiveGameContext, we store the handle of the event binding instead.
	TArray<uint32> ActiveActionHandles;
	for (TPair<UInputAction*, TSubclassOf<UGameplayAbility>> AbilityBinding : GameContext->AbilityBindings)
	{
		FGameplayAbilitySpecHandle Handle = AbilitySystemComponent->GiveAbility(FGameplayAbilitySpec(AbilityBinding.Value));
		ActiveAbilityHandles.Add(Handle);
		UE_LOG(LogToybox, Display, TEXT("%hs - Granted ability: %s"), __FUNCTION__, *AbilityBinding.Value->GetName());

		// This creates a lambda which maps the input action to the activate ability function so we no longer need a direct function reference
		FEnhancedInputActionEventBinding& ActionBinding = EnhancedInputComponent->BindActionInstanceLambda(AbilityBinding.Key, ETriggerEvent::Triggered, [AbilitySystemComponent, Handle](const FInputActionInstance& Instance)
		{
			AbilitySystemComponent->TryActivateAbility(Handle);
		});

		ActiveActionHandles.Add(ActionBinding.GetHandle());
		UE_LOG(LogToybox, Display, TEXT("%hs - Bound input action: %s to ability: %s"), __FUNCTION__, *AbilityBinding.Key->GetName(), *AbilityBinding.Value->GetName());
	}

	FActiveGameContext ActiveGameContext;
	ActiveGameContext.Context = GameContext;
	ActiveGameContext.GrantedAbilityHandles = ActiveAbilityHandles;
	ActiveGameContext.GrantedActionHandles = ActiveActionHandles;

	ToyboxPlayerState->AddActiveGameContext(ActiveGameContext);
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

	for (FGameplayAbilitySpecHandle ActiveHandle : ActiveGameContext.GrantedAbilityHandles)
	{
		AbilitySystemComponent->ClearAbility(ActiveHandle);
	}

	ToyboxPlayerState->RemoveActiveGameContext(GameContext);
}
