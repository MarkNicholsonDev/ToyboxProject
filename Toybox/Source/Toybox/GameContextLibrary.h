// Copyright 2026 Mark Nicholson. All Rights Reserved.

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
