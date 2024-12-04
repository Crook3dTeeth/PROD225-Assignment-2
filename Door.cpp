// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"
#include "PlayerCharacter.h"

// Sets default values
ADoor::ADoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.bStartWithTickEnabled = false;
	SetActorTickEnabled(false);

	DoorMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("DoorMesh"));

	ClosedState = true;

	CollisionCapsule = CreateDefaultSubobject<UCapsuleComponent>(FName("CollisionCapsule"));
	CollisionCapsule->AttachToComponent(DoorMesh, FAttachmentTransformRules::KeepRelativeTransform);
}


// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	// Set the initial rotation
	SetActorRotation(ClosedRotation);
	Super::BeginPlay();

	// Add door collision delegate and add it to the door collision capsule
	FScriptDelegate OnDoorCollision;
	OnDoorCollision.BindUFunction(this, FName("DoorCollision"));
	CollisionCapsule->OnComponentBeginOverlap.Add(OnDoorCollision);
}



void ADoor::OnConstruction(const FTransform& Transform)
{
	// Set the door rotation based on its closed/open state
	if (ClosedState) {
		SetActorRotation(ClosedRotation);
	}
	else {
		SetActorRotation(OpenRotation);
	}
}


// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!ClosedState) {
		
		// Get the current rotation
		FRotator CurrentRotation = GetActorRotation();
		// Get new rotation
		double YawRotation = FMath::Lerp(CurrentRotation.Yaw, OpenRotation.Yaw, DeltaTime);
		CurrentRotation.Yaw = YawRotation;

		// Set rotation
		SetActorRotation(CurrentRotation);

		// Once door is 'close enough' to new state snap to correct state and disable ticking
		if (abs(CurrentRotation.Yaw - OpenRotation.Yaw) < 0.4) {
			SetActorRotation(OpenRotation);
			SetActorTickEnabled(false);
		}
	}
}



void ADoor::DoorCollision(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check collision was the player and a collision capsule
	if (Cast<APlayerCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {
		// Check if door is closed
		if (!ClosedState) {
			return;
		}
		// Enable door tick
		SetActorTickEnabled(true);

		// Set door state
		ClosedState = false;
	}
}
