// Fill out your copyright notice in the Description page of Project Settings.

#include "LevelBuilder.h"
#include "Kismet/GameplayStatics.h"
#include "PlayerCharacter.h"
#include "Room.h"


// Sets default values
ALevelBuilder::ALevelBuilder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ALevelBuilder::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ALevelBuilder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}



void ALevelBuilder::OnConstruction(const FTransform& Transform)
{
	// Gets the spawned room pieces and destroys them
	TArray<AActor*> FoundRoomPieces;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ARoomPiece::StaticClass(), FoundRoomPieces);
	for (AActor* RoomPiece : FoundRoomPieces)
	{
		RoomPiece->Destroy();
	}

	// Delete the room piece objects
	for (int32 RoomIndex = 0; RoomIndex < ConstructedRooms.Num(); RoomIndex++) {
		ConstructedRooms[RoomIndex]->DestroyRoom();
	}
	ConstructedRooms.Empty();


	// Build the new rooms
	for (int32 RoomIndex = 0; RoomIndex < Rooms.Num(); RoomIndex++) {

		ARoom* NewRoom = NewObject<ARoom>(this);

		NewRoom->BuildRoom(&Rooms[RoomIndex], GetActorLocation());
	}
}
