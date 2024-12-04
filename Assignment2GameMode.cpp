// Copyright Epic Games, Inc. All Rights Reserved.

#include "Assignment2GameMode.h"
#include "PlayerCharacter.h"


AAssignment2GameMode::AAssignment2GameMode()
{
	// set default pawn class to our character class
	DefaultPawnClass = APlayerCharacter::StaticClass();
}

void AAssignment2GameMode::SendCharacterMessage(const FString& NewMessage)
{
	OnTextMessageRecieved.Broadcast(NewMessage);
}

