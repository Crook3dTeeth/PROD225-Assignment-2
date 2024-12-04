// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyCharacter.h"
#include "EnemySpawner.generated.h"

UCLASS()
class ASSIGNMENT2_API AEnemySpawner : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemySpawner();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	void KillActor();

	uint32 TotalSpawned = 0;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	FTimerHandle SpawnTimerHandle;

	void SpawnEnemy();

	UPROPERTY(EditAnywhere)
	TSubclassOf<AEnemyCharacter> ClassToSpawn;

	UPROPERTY(EditAnywhere)
	uint32 SpawnLimit = 3;

	UPROPERTY(EditAnywhere)
	float SpawnTimer = 4.0f;

	UPROPERTY(EditAnywhere)
	uint32 LifeSpawnLimit = 5;
};
