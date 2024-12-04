// Fill out your copyright notice in the Description page of Project Settings.


#include "Room.h"
#include "Door.h"
#include "RoomPiece.h"
#include "RoomStyle.h"

// Sets default values
ARoom::ARoom()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}



// Called when the game starts or when spawned
void ARoom::BeginPlay()
{
	Super::BeginPlay();
	
}


void GetWallRotation(FRotator* Rotation, int32 RoomX, int32 RoomY, int32 RoomWidth, int32 RoomLength)
{
	if (RoomX == 0 && RoomY != 0) {
		// Bottom side
		Rotation->Yaw = 270;
	}
	else if (RoomX != 0 && RoomY == 0) {
		//Left side
		//Rotation->Yaw += 270;
	}
	else if (RoomX == RoomWidth - 1) {
		// Top side
		Rotation->Yaw = 90;
	}
	else {
		Rotation->Yaw = 180;
	}
}


void GetCornerRotation(FRotator* Rotation, int32 RoomX, int32 RoomY, int32 RoomWidth, int32 RoomLength)
{
	if (RoomX == 0 && RoomY != 0) {
		Rotation->Yaw = 270;
	}
	else if (RoomX != 0 && RoomY == 0) {
		Rotation->Yaw = 90;
	}
	else if (RoomX == RoomWidth - 1) {
		Rotation->Yaw = 180;
	}
}


void GetDoorRotation(FRotator* Rotation, int32 RoomX, int32 RoomY, int32 RoomWidth, int32 RoomLength)
{
	if (RoomX == 0 && RoomY != 0) {
		// Bottom side
		Rotation->Yaw = 270;
	}
	else if (RoomX != 0 && RoomY == 0) {
		//Left side
		//Rotation->Yaw += 270;
	}
	else if (RoomX == RoomWidth - 1) {
		// Top side
		Rotation->Yaw = 90;
	}
	else {
		Rotation->Yaw = 180;
	}
}


/// <summary>
/// Gets the correct piece to spawn based on room dimensions and sets the correct rotation
/// </summary>
void PieceSelector(TSubclassOf<URoomStyle>* RoomStyle, FRotator* Rotation, TSubclassOf<ARoomPiece>* PieceToSpawn, TArray<FDoorConfiguration> Doors, int32 RoomX, int32 RoomY, int32 RoomWidth, int32 RoomLength)
{
	// Check for corner
	if ((RoomX == 0 && RoomY == 0) || (RoomY == 0 && RoomX == RoomWidth - 1) || (RoomX == 0 && RoomY == RoomLength - 1) || (RoomY == RoomLength - 1 && RoomX == RoomWidth - 1))
	{
		*PieceToSpawn = RoomStyle->GetDefaultObject()->Corner;
		GetCornerRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
	}
	// Check if wall/edge and then if door
	else if (RoomX == 0 || RoomY == 0 || RoomX == RoomWidth - 1 || RoomY == RoomLength - 1)
	{
		//Set defualt wall
		*PieceToSpawn = RoomStyle->GetDefaultObject()->Walls;
		GetWallRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
		if (Doors.Num() > 0) {

			// Iterate through any doors and check if part is door
			for (int32 DoorIndex = 0; DoorIndex < Doors.Num(); DoorIndex++) {
				switch (Doors[DoorIndex].Wall)
				{
				case EWall::NORTH:
					if (RoomY == RoomLength - 1 && Doors[DoorIndex].DistanceAlongWall == RoomX)
					{
						*PieceToSpawn = RoomStyle->GetDefaultObject()->DoorFrame;
						GetDoorRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
						return;
					}
					break;
				case EWall::EAST:
					if (RoomX == RoomWidth - 1 && Doors[DoorIndex].DistanceAlongWall == RoomY)
					{
						*PieceToSpawn = RoomStyle->GetDefaultObject()->DoorFrame;
						GetDoorRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
						return;
					}
					break;
				case EWall::SOUTH:
					if (RoomY == 0 && Doors[DoorIndex].DistanceAlongWall == RoomX)
					{
						*PieceToSpawn = RoomStyle->GetDefaultObject()->DoorFrame;
						GetDoorRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
						return;
					}
					break;
				case EWall::WEST:
					if (RoomX == 0 && Doors[DoorIndex].DistanceAlongWall == RoomY)
					{
						*PieceToSpawn = RoomStyle->GetDefaultObject()->DoorFrame;
						GetDoorRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
						return;
					}
					break;
				default:
					break;
				}
			}
		}
		else { // 
			*PieceToSpawn = RoomStyle->GetDefaultObject()->Walls;
			GetWallRotation(Rotation, RoomX, RoomY, RoomWidth, RoomLength);
			return;
		}
	}
	else {
		*PieceToSpawn = RoomStyle->GetDefaultObject()->Floor;
		return;
	}
}


void ARoom::BuildRoom(FRoomConfiguration* RoomConfig, FVector Offset)
{
	
	if (RoomConfig->RoomLength <= 1 || RoomConfig->RoomWidth <= 1) {
		return;
	}

	if (!RoomConfig->RoomStyle) {
		return;
	}

	// Get the world
	if (UWorld* World = GetWorld()) {
		// Loop over room layout
		for (int32 RoomX = 0; RoomX < RoomConfig->RoomWidth; RoomX++) {
			for (int32 RoomY = 0; RoomY < RoomConfig->RoomLength; RoomY++) {
				// Get location of part
				FVector Location = FVector(RoomX * 500 + (500 * RoomConfig->PositionX) + Offset.X, RoomY * 500 + (500* RoomConfig->PositionY) + Offset.Y, 0 + Offset.Z);
				FRotator Rotation = FRotator(0, 0, 0);

				TSubclassOf<ARoomPiece>* PieceToSpawn = new TSubclassOf<ARoomPiece>();
				// Set the correct piece
				PieceSelector(&RoomConfig->RoomStyle, &Rotation, PieceToSpawn, RoomConfig->Doors,RoomX, RoomY, RoomConfig->RoomWidth, RoomConfig->RoomLength);
				// Spawn the piece
				ARoomPiece* NewPiece = World->SpawnActor<ARoomPiece>(*PieceToSpawn, Location, Rotation);
					
				if (NewPiece) {
					RoomPieces.Add(NewPiece);
				}
			}
		}
	}
}


/// <summary>
/// Deletes stored pointers for the room pieces.
/// </summary>
void ARoom::DestroyRoom()
{
	for (int PieceIndex = 0; PieceIndex < RoomPieces.Num(); PieceIndex++) {
		if (RoomPieces[PieceIndex]) {
			delete RoomPieces[PieceIndex];
		}
	}

	RoomPieces.Empty();
}


// Called every frame
void ARoom::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

