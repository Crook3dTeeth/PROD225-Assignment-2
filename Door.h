// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"


UENUM(BlueprintType)
enum class EWall : uint8
{
	NORTH = 0,
	EAST = 1,
	SOUTH = 2,
	WEST = 3
};


/// <summary>
/// Used for room builder to store location data in the room
/// </summary>
USTRUCT(BlueprintType)
struct FDoorConfiguration
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EWall Wall;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 DistanceAlongWall;
};



UCLASS()
class ASSIGNMENT2_API ADoor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADoor();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool ClosedState;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator ClosedRotation;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FRotator OpenRotation;

private:

	UFUNCTION()
	void DoorCollision(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* CollisionCapsule;
};
