// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Assignment2GameMode.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnTextMessageRecieved, FString, Message);

UCLASS(MinimalAPI)
class AAssignment2GameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AAssignment2GameMode();

	UPROPERTY(BlueprintAssignable)
	FOnTextMessageRecieved OnTextMessageRecieved;

	UFUNCTION(BlueprintCallable)
	void SendCharacterMessage(const FString& NewMessage);
};



