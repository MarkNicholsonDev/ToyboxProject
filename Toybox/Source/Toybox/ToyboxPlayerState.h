// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemInterface.h"
#include "AbilitySystemComponent.h"
#include "GameFramework/PlayerState.h"
#include "GameContext.h"
#include "ToyboxPlayerState.generated.h"

/**
 * 
 */
UCLASS()
class TOYBOX_API AToyboxPlayerState : public APlayerState, public IAbilitySystemInterface
{
	GENERATED_BODY()
	
public:
	/** Constructor */
	AToyboxPlayerState();

	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	void AddActiveGameContext(const UGameContext* GameContext);
	void RemoveActiveGameContext(const UGameContext* GameContext);
	TArray<const UGameContext*> GetActiveGameContexts();

protected:

private:
	UPROPERTY()
	UAbilitySystemComponent* ASC;

	UPROPERTY(Replicated)
	TArray<const UGameContext*> ActiveGameContexts;

	bool bActiveGameContextsDirty = false;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
};
