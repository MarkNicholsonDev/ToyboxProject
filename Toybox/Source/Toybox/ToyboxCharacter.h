// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "Logging/LogMacros.h"
#include "Delegates/DelegateCombinations.h"
#include "ToyboxCharacter.generated.h"

class UInputComponent;
class UAbilitySystemComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UGameContext;
class UInputAction;
struct FInputActionValue;

// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnInteractDelegate);

/**
 *  A basic first person character
 */
UCLASS(abstract)
class AToyboxCharacter : public ACharacter, public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	/** Pawn mesh: first person view (arms; seen only by self) */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* FirstPersonMesh;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractDelegate OnInteractDelegate;

	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnInteractDelegate ExitContextDelegate;

	AToyboxCharacter();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	/** Returns the first person mesh **/
	USkeletalMeshComponent* GetFirstPersonMesh() const { return FirstPersonMesh; }

	/** Returns first person camera component **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

	UFUNCTION(Client, Reliable)
	void Client_AddGameContextInputBindings(const UGameContext* GameContext, AToyboxPlayerController* PlayerController);

protected:
	/** Jump Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* JumpAction;

	/** Move Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	UInputAction* MoveAction;

	/** Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* LookAction;

	/** Mouse Look Input Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* MouseLookAction;

	/** Interact Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* InteractAction;

	/** End Interact Action */
	UPROPERTY(EditAnywhere, Category = "Input")
	class UInputAction* ExitContextAction;

	/** Called from Input Actions for movement input */
	void MoveInput(const FInputActionValue& Value);

	/** Called from Input Actions for looking input */
	void LookInput(const FInputActionValue& Value);

	/** Called from Input Actions for interaction input */
	void StartInteraction(const FInputActionValue& Value);

	/** Called from Input Actions for exiting interactions and closing any UI widget contexts */
	void ExitContext(const FInputActionValue& Value);

	/** Handles aim inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoAim(float Yaw, float Pitch);

	/** Handles move inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoMove(float Right, float Forward);

	/** Handles jump start inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpStart();

	/** Handles jump end inputs from either controls or UI interfaces */
	UFUNCTION(BlueprintCallable, Category="Input")
	virtual void DoJumpEnd();

	/** Gameplay initialization */
	virtual void BeginPlay() override;

	/** Set up input action bindings */
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

private:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	UGameContext* DefaultCharacterContext = nullptr;

	virtual void PossessedBy(AController* NewController) override;

	virtual void OnRep_PlayerState() override;
};

