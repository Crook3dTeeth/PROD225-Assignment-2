// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "RoomPiece.h"
#include "RoomStyle.generated.h"




UCLASS(Blueprintable)
class ASSIGNMENT2_API URoomStyle : public UObject
{
	GENERATED_BODY()
	
	URoomStyle();

public:	


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARoomPiece> Walls;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARoomPiece> Corner;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARoomPiece> DoorFrame;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<ARoomPiece> Floor;
};
