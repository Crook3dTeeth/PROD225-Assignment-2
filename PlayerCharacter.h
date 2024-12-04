// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Item.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"

UCLASS()
class ASSIGNMENT2_API APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APlayerCharacter();

protected:
	// Called when the game starts or when actor is spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere)
	UNiagaraSystem* SwordParticles;


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PlayerHealth = 100.0f;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual float TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/**
	* Interrupt / stop the attack animation if it is currently playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void StopAttacking();

	/**
	* Query the player to see if the attack animation is playing
	*/
	UFUNCTION(BlueprintImplementableEvent)
	bool IsAttacking();

	/**
	* Causes the player mesh to ragdoll, the sword to drop and simulate physics and disables the movement component. RIP player :'(
	*/
	UFUNCTION(BlueprintImplementableEvent)
	void Ragdoll();

	// Player inventory
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<TSubclassOf<AItem>> PlayerInventory;

private:

	FTimerHandle QuitHandle;

	void Quit();

	UPROPERTY(EditAnywhere)
	float SwordDamage = 20.0f;

	UFUNCTION()
	void AttackSword(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit);


	/**
	* Listen to this sword's overlap events to deal damage to enemies.
	*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* SwordMesh;

};
