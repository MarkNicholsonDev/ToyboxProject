// Copyright Epic Games, Inc. All Rights Reserved.


#include "ToyboxPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "Engine/LocalPlayer.h"
#include "InputMappingContext.h"
#include "ToyboxCameraManager.h"
#include "AbilitySystemComponent.h"
#include "ToyboxPlayerState.h"
#include "ToyboxCharacter.h"
#include "Blueprint/UserWidget.h"
#include "Toybox.h"
#include "Widgets/Input/SVirtualJoystick.h"

AToyboxPlayerController::AToyboxPlayerController()
{
	// set the player camera manager class
	PlayerCameraManagerClass = AToyboxCameraManager::StaticClass();
}

void AToyboxPlayerController::AddMappingContext(UInputMappingContext* MappingContext)
{
	if(AdditionalMappingContexts.Contains(MappingContext))
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Mapping context already added to player controller"), __FUNCTION__);
		return;
	}

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(MappingContext, 0);

		AdditionalMappingContexts.Add(MappingContext);
	}
}

void AToyboxPlayerController::RemoveMappingContext(UInputMappingContext* MappingContext)
{
	if (!AdditionalMappingContexts.Contains(MappingContext))
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Mapping context already added to player controller"), __FUNCTION__);
		return;
	}

	// Add Input Mapping Context
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		Subsystem->RemoveMappingContext(MappingContext);

		AdditionalMappingContexts.Remove(MappingContext);
	}
}

void AToyboxPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// only spawn touch controls on local player controllers
	if (ShouldUseTouchControls() && IsLocalPlayerController())
	{
		// spawn the mobile controls widget
		MobileControlsWidget = CreateWidget<UUserWidget>(this, MobileControlsWidgetClass);

		if (MobileControlsWidget)
		{
			// add the controls to the player screen
			MobileControlsWidget->AddToPlayerScreen(0);

		} else {

			UE_LOG(LogToybox, Error, TEXT("Could not spawn mobile controls widget."));

		}

	}
}

void AToyboxPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	// only add IMCs for local player controllers
	if (IsLocalPlayerController())
	{
		// Add Input Mapping Context
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
		{
			for (UInputMappingContext* CurrentContext : DefaultMappingContexts)
			{
				Subsystem->AddMappingContext(CurrentContext, 0);
			}

			// only add these IMCs if we're not using mobile touch input
			if (!ShouldUseTouchControls())
			{
				for (UInputMappingContext* CurrentContext : MobileExcludedMappingContexts)
				{
					Subsystem->AddMappingContext(CurrentContext, 0);
				}
			}
		}
	}
	
}

bool AToyboxPlayerController::ShouldUseTouchControls() const
{
	// Check on client side if are we on a mobile platform and if so displace touch controls or should we force touch controls?
	if (IsLocalController())
	{
		return SVirtualJoystick::ShouldDisplayTouchInterface() || bForceTouchControls;
	}
	return false;
}

void AToyboxPlayerController::AcknowledgePossession(APawn* PossessedPawn)
{
	Super::AcknowledgePossession(PossessedPawn);

	AToyboxCharacter* ToyboxCharacter = Cast<AToyboxCharacter>(PossessedPawn);
	if (ToyboxCharacter == nullptr)
	{
		UE_LOG(LogToybox, Warning, TEXT("%hs - Failed to cast pawn to a ToyboxCharacter, might need to swap the possessed pawn to a Toybox derived character for ASC functionality."), __FUNCTION__);
		return;
	}

	UAbilitySystemComponent* ASC = ToyboxCharacter->GetAbilitySystemComponent();
	if (ASC == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid ASC cast from character"), __FUNCTION__);
		return;
	}

	AToyboxPlayerState* ToyboxPlayerState = GetPlayerState<AToyboxPlayerState>();
	if (ToyboxPlayerState == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerState cast"), __FUNCTION__);
		return;
	}

	ASC->InitAbilityActorInfo(ToyboxPlayerState, ToyboxCharacter);
}
