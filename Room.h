// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "RoomPiece.h"
#include "Door.h"
#include "RoomStyle.h"
#include "Room.generated.h"


USTRUCT(BlueprintType)
struct FRoomConfiguration
{
	GENERATED_BODY()

	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin=0))
	int32 RoomWidth;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, meta=(ClampMin = 0))
	int32 RoomLength;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<URoomStyle> RoomStyle;
	

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PositionX;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int32 PositionY;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FDoorConfiguration> Doors;
};


UCLASS()
class ASSIGNMENT2_API ARoom : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ARoom();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	

public:	
	TArray<ARoomPiece*> RoomPieces;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void BuildRoom(FRoomConfiguration* RoomConfig, FVector Offset);

	void DestroyRoom();

	uint16 RoomWidth = 0;

	uint16 RoomLength = 0;

	
};
