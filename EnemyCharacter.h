// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "PlayerCharacter.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

UCLASS()
class ASSIGNMENT2_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Bind to the Overlap and Endoverlap delegates of this component to run logic to go to the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* PlayerDetectionSphere;

	// Bind to the Overlap and Endoverlap delegate of this component to run logic to attack the player
	UPROPERTY(EditAnywhere)
	class USphereComponent* AttackRadiusSphere;

	UPROPERTY(EditAnywhere)
	float AttackRadius = 100.f;

	UPROPERTY(EditAnywhere)
	float PlayerDetectionRadius = 500.f;

	UPROPERTY(EditAnywhere)
	float Health = 100.0f;

	UPROPERTY(EditAnywhere)
	float EnemyDamage = 15.0f;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;


	UFUNCTION()
	void HandlePlayerDetection(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


	/**
	* Cause a single attack animation loop to play
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StartAttacking();

	/**
	* Interrupt / stop the attack animation if it is currently playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StopAttacking();

	/**
	* Query the Enemy to see if the attack animation is playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	bool IsAttacking();


	UFUNCTION()
	void HandlePlayerAttack(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);



	UFUNCTION()
	void PlayerLost(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

	/*
	Updates the enemy spawner on enemy elimination
	*/
	DECLARE_DELEGATE(FOnEnemyElimintated);

	FOnEnemyElimintated OnEnemyEliminated;


	UFUNCTION()
	void StopAttackingPlayer(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	Handles when enemy is attacked by a sword
	*/
	UFUNCTION()
	void HandleAttack();

	APlayerCharacter* TargetOfInterest;

	bool PlayerDetected = false;

	bool AttackPlayer = false;

	FTimerHandle AttackTimerHandle;

private:
	FTimerManager* AttackTimerManager;
};
