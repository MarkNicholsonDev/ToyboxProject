// Copyright Epic Games, Inc. All Rights Reserved.

#include "ToyboxCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "ToyboxPlayerState.h"
#include "Net/UnrealNetwork.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "GameContextLibrary.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/PlayerController.h"
#include "ToyboxPlayerController.h"
#include "Toybox.h"

AToyboxCharacter::AToyboxCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

UAbilitySystemComponent* AToyboxCharacter::GetAbilitySystemComponent() const
{
	return ASC;
}

void AToyboxCharacter::BeginPlay()
{
	Super::BeginPlay();

}

void AToyboxCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AToyboxCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AToyboxCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AToyboxCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AToyboxCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AToyboxCharacter::LookInput);
	}
	else
	{
		UE_LOG(LogToybox, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}

	Server_AddDefaultGameContext();
}

void AToyboxCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	AToyboxPlayerState* ToyboxPlayerState = GetPlayerState<AToyboxPlayerState>();
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

	ASC = AbilitySystemComponent;
	ASC->InitAbilityActorInfo(ToyboxPlayerState, this);
}

void AToyboxCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void AToyboxCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void AToyboxCharacter::StartInteraction(const FInputActionValue& Value)
{
	if (OnInteractDelegate.IsBound())
	{
		OnInteractDelegate.Broadcast();
	}
}

void AToyboxCharacter::ExitContext(const FInputActionValue& Value)
{
	// Cancel any interactions or close any UI widgets
	if (ExitContextDelegate.IsBound())
	{
		ExitContextDelegate.Broadcast();
	}
}

void AToyboxCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AToyboxCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AToyboxCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AToyboxCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AToyboxCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	AToyboxPlayerState* ToyboxPlayerState = GetPlayerState<AToyboxPlayerState>();
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

	ASC = AbilitySystemComponent;
	ASC->InitAbilityActorInfo(ToyboxPlayerState, this);
}

void AToyboxCharacter::Client_AddGameContextInputBindings_Implementation(const UGameContext* GameContext, AToyboxPlayerState* TBPlayerState)
{
	AToyboxPlayerController* PlayerController = Cast<AToyboxPlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerController cast"), __FUNCTION__);
		return;
	}

	AToyboxPlayerState* ToyboxPlayerState = TBPlayerState;
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

	TArray<uint32> ActionHandles;
	for (FGameContextAbilityBinding AbilityBinding : GameContext->AbilityBindings)
	{
		// This creates a lambda which maps the input action to the activate ability function so we no longer need a direct function reference
		FEnhancedInputActionEventBinding& ActionBinding = EnhancedInputComponent->BindActionInstanceLambda(AbilityBinding.InputAction, ETriggerEvent::Triggered, [AbilitySystemComponent, Tag = AbilityBinding.AbilityTag](const FInputActionInstance& Instance)
			{
				AbilitySystemComponent->TryActivateAbilitiesByTag(FGameplayTagContainer(Tag));
			});

		ActionHandles.Add(ActionBinding.GetHandle());
		UE_LOG(LogToybox, Verbose, TEXT("%hs - Bound input action: %s to ability: %s"), __FUNCTION__, *AbilityBinding.InputAction->GetName(), *AbilityBinding.AbilityClass->GetName());
	}

	FActiveGameContextActionHandles ActiveGameContext;
	ActiveGameContext.Context = GameContext;
	ActiveGameContext.GrantedActionHandles = ActionHandles;

	LocalGameContexts.Add(ActiveGameContext);
}

void AToyboxCharacter::Client_RemoveGameContextInputBindings_Implementation(const UGameContext* GameContext)
{
	AToyboxPlayerController* PlayerController = Cast<AToyboxPlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid PlayerController cast"), __FUNCTION__);
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

	UE_LOG(LogToybox, Verbose, TEXT("%hs - Removed mapping context: %s and loose gameplay tag: %s"), __FUNCTION__, *GameContext->MappingContext->GetName(), *GameContext->GameContextTag.ToString());

	FActiveGameContextActionHandles ActiveGameContext;

	if (!GetActiveGameContextActionHandles(GameContext, ActiveGameContext))
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Cannot find the active game context for game context: %s"), __FUNCTION__, *GameContext->MappingContext->GetName());
		return;
	}

	for (uint32 ActionHandle : ActiveGameContext.GrantedActionHandles)
	{
		EnhancedInputComponent->RemoveBindingByHandle(ActionHandle);
		UE_LOG(LogToybox, Verbose, TEXT("%hs - Removed input action handle: %d from ActiveGameContext for: %s"), __FUNCTION__, ActionHandle, *GameContext->GetName());
	}

	LocalGameContexts.RemoveAll([GameContext](const FActiveGameContextActionHandles& Context)
		{
			return Context.Context == GameContext;
		});
}

bool AToyboxCharacter::GetActiveGameContextActionHandles(const UGameContext* GameContext, FActiveGameContextActionHandles& OutContext)
{
	for (FActiveGameContextActionHandles& ActiveGameContext : LocalGameContexts) 
	{
		if (ActiveGameContext.Context == GameContext)
		{
			OutContext = ActiveGameContext;
			return true;
		}
	}
	return false;
}

void AToyboxCharacter::RemoveActiveGameContextAbilityHandles(const UGameContext* GameContext)
{
	ServerGameContexts.RemoveAll([GameContext](const FActiveGameContextAbilityHandles& Context)
		{
			return Context.Context == GameContext;
		});
}

void AToyboxCharacter::AddActiveGameContextAbilityHandles(FActiveGameContextAbilityHandles ActiveGameContextHandles)
{
	ServerGameContexts.Add(ActiveGameContextHandles);
}

bool AToyboxCharacter::IsGameContextActive(const UGameContext* GameContext)
{
	for (FActiveGameContextActionHandles& LocalGameContext : LocalGameContexts)
	{
		if (LocalGameContext.Context == GameContext) 
		{
			return true;
		}
	}
	return false;
}

bool AToyboxCharacter::GetActiveGameContextAbilityHandles(const UGameContext* GameContext, FActiveGameContextAbilityHandles& OutContext)
{
	for (FActiveGameContextAbilityHandles& ActiveGameContext : ServerGameContexts)
	{
		if (ActiveGameContext.Context == GameContext)
		{
			OutContext = ActiveGameContext;
			return true;
		}
	}
	return false;
}

void AToyboxCharacter::Server_AddDefaultGameContext_Implementation()
{
	// Add default abilities via a game context to the player character
	AToyboxPlayerController* PlayerController = Cast<AToyboxPlayerController>(GetController());
	if (PlayerController == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid player controller fetched"), __FUNCTION__);
		return;
	}

	if (DefaultCharacterContext == nullptr)
	{
		UE_LOG(LogToybox, Error, TEXT("%hs - Invalid DefaultCharacterContext"), __FUNCTION__);
		return;
	}

	UGameContextLibrary::ApplyGameContext(DefaultCharacterContext, PlayerController);
}

void AToyboxCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
}
