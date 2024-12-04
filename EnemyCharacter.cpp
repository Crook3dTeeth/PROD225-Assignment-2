// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemyCharacter.h"
#include "PlayerCharacter.h"
#include "Components/SphereComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"

// Sets default values
AEnemyCharacter::AEnemyCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	PlayerDetectionSphere = CreateDefaultSubobject<USphereComponent>(FName("PlayerDetectionSphere"));
	AttackRadiusSphere = CreateDefaultSubobject<USphereComponent>(FName("AttackRadiusSphere"));

	if (RootComponent)
	{
		if (AttackRadiusSphere)
		{
			AttackRadiusSphere->SetSphereRadius(AttackRadius);

			// Make sure it's attached to the root component
			AttackRadiusSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
		
		if (PlayerDetectionSphere)
		{
			PlayerDetectionSphere->SetSphereRadius(PlayerDetectionRadius);

			// Make sure it's attached to the root component
			PlayerDetectionSphere->AttachToComponent(RootComponent, FAttachmentTransformRules::KeepRelativeTransform);
		}
	}


}

// Called when the game starts or when spawned
void AEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();


	// Delegate for when player enters detection radius
	FScriptDelegate OnPlayerLocated;
	OnPlayerLocated.BindUFunction(this, FName("HandlePlayerDetection"));
	PlayerDetectionSphere->OnComponentBeginOverlap.Add(OnPlayerLocated);

	// Player left range
	FScriptDelegate OnPlayerLost;
	OnPlayerLost.BindUFunction(this, FName("PlayerLost"));
	PlayerDetectionSphere->OnComponentEndOverlap.Add(OnPlayerLost);

	// Player enters attack range
	FScriptDelegate AttackPlayerRange;
	AttackPlayerRange.BindUFunction(this, FName("HandlePlayerAttack"));
	AttackRadiusSphere->OnComponentBeginOverlap.Add(AttackPlayerRange);

	// Player left attack range
	FScriptDelegate StopAttackingRange;
	StopAttackingRange.BindUFunction(this, FName("StopAttackingPlayer"));
	AttackRadiusSphere->OnComponentEndOverlap.Add(StopAttackingRange);


	// Setup attack timer delegate
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		AttackTimerManager = &GameInstance->GetTimerManager();

		FTimerDelegate AttackTimerDelegate;

		AttackTimerDelegate.BindUObject(this, &AEnemyCharacter::HandleAttack);

		AttackTimerManager->SetTimer(AttackTimerHandle, AttackTimerDelegate, 1.6f, true);

		AttackTimerManager->PauseTimer(AttackTimerHandle);
	}
}


// Called every frame
// Moves the enemy towards 
void AEnemyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (PlayerDetected && !AttackPlayer) {
		GetCharacterMovement()->AddInputVector((TargetOfInterest->GetActorLocation() - GetActorLocation()));
	}

}


// Called to bind functionality to input
void AEnemyCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}


void AEnemyCharacter::HandleAttack()
{
	StartAttacking();

	TargetOfInterest->TakeDamage(EnemyDamage, FDamageEvent(), GetController(), this);
}


void AEnemyCharacter::HandlePlayerAttack(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check collision was the player and a collision capsule
	if (Cast<APlayerCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {
		AttackPlayer = true;
		AttackTimerManager->UnPauseTimer(AttackTimerHandle);
	}
}


void AEnemyCharacter::StopAttackingPlayer(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check collision was the player and a collision capsule
	if (Cast<APlayerCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {
		AttackPlayer = false;
		AttackTimerManager->PauseTimer(AttackTimerHandle);
	}
}


void AEnemyCharacter::HandlePlayerDetection(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	// Check collision was the player and a collision capsule
	if (Cast<APlayerCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {

		TargetOfInterest = Cast<APlayerCharacter>(SelfActor);
		PlayerDetected = true;
	}
}


void AEnemyCharacter::PlayerLost(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	if (Cast<APlayerCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {
		PlayerDetected = false;
	}
}


float AEnemyCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CanBeDamaged()) {
		Health -= Damage;

		if (Health <= 0) {
			SetCanBeDamaged(false);
		
			return Damage + Health;
		}
		else {
			return Damage;
		}
	}
	return 0.0f;
}