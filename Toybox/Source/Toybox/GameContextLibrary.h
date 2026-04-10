// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameContextLibrary.generated.h"

class AToyboxPlayerController;
class UGameContext;

/**
 * 
 */
UCLASS()
class TOYBOX_API UGameContextLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	UFUNCTION(Category = "Game Context")
	static void ApplyGameContext(const UGameContext* GameContext, AToyboxPlayerController* PlayerController);

	UFUNCTION(Category = "Game Context")
	static void RemoveGameContext(const UGameContext* GameContext, AToyboxPlayerController* PlayerController);

};
