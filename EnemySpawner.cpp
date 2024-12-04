// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawner.h"

// Sets default values
AEnemySpawner::AEnemySpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AEnemySpawner::BeginPlay()
{
	Super::BeginPlay();

	// Set the spawn timer 
	if (UGameInstance* GameInstance = GetGameInstance())
	{
		FTimerManager& TimerManager = GameInstance->GetTimerManager();

		FTimerDelegate TimerDelegate;

		TimerDelegate.BindUObject(this, &AEnemySpawner::SpawnEnemy);


		TimerManager.SetTimer(SpawnTimerHandle, TimerDelegate, SpawnTimer, true);
	}	
}

// Called every frame
void AEnemySpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


/// <summary>
/// Spawns a new enemy
/// </summary>
void AEnemySpawner::SpawnEnemy()
{
	// Check if world limit and total spawn limit is reached
	if (TotalSpawned < SpawnLimit && TotalSpawned < LifeSpawnLimit) {
		const FVector Location = this->GetActorLocation();
		const FRotator Rotation = this->GetActorRotation();

		if (UWorld* World = GetWorld())
		{
			if (AEnemyCharacter* ASpawnedEnemy = Cast<AEnemyCharacter>(World->SpawnActor<AActor>(ClassToSpawn, Location, Rotation)))
			{
				ASpawnedEnemy->OnEnemyEliminated.BindUObject(this, &AEnemySpawner::KillActor);
			}
			TotalSpawned += 1;
		}
	}
}


void AEnemySpawner::KillActor()
{
	if (TotalSpawned > 0) {
		TotalSpawned--;
	}
}

