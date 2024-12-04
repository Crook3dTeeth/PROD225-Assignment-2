// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.h"
#include "PlayerCharacter.h"
#include "Room.h"
#include "LevelBuilder.generated.h"



UCLASS(BlueprintType)
class ASSIGNMENT2_API ALevelBuilder : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelBuilder();

	UPROPERTY(EditAnywhere)
	TArray<ARoom*> ConstructedRooms;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<ARoom> RoomClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FRoomConfiguration> Rooms;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
