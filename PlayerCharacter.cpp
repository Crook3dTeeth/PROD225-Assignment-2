// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerCharacter.h"

#include "Assignment2GameMode.h"
#include "Math/UnrealMathUtility.h"
#include "EnemyCharacter.h"
#include "NiagaraFunctionLibrary.h"
#include "NiagaraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
APlayerCharacter::APlayerCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Attach a sword mesh to the character mesh
	SwordMesh = CreateDefaultSubobject<UStaticMeshComponent>(FName("SwordMesh"));
	SwordMesh->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, FName("Hand_R"));
}

// Called when the game starts or when spawned
void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Sword collision delegate
	FScriptDelegate OnSwordHitDelegate;
	OnSwordHitDelegate.BindUFunction(this, FName("AttackSword"));
	SwordMesh->OnComponentBeginOverlap.Add(OnSwordHitDelegate);
}

// Called every frame
void APlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}



void APlayerCharacter::AttackSword(AActor* ActorHit, AActor* SelfActor, AActor* OtherActor, FVector NormalImpulse, const FHitResult& Hit)
{
	
	// Check if sword hit enemy capsule during an attack
	if (IsAttacking() && Cast<AEnemyCharacter>(SelfActor) && Cast<UCapsuleComponent>(OtherActor)) {
		StopAttacking();


		float DamageDealt = SelfActor->TakeDamage(SwordDamage, FDamageEvent(), GetController(), this);

		// Check if any damage was dealt for damage message
		if (DamageDealt <= 0.0f) {
			SelfActor->Destroy();
			return;
		}
	

		// Check if sword particles has been set
		if (SwordParticles) {
			// Generate random yaw direction for the paritcles
			FRotator ParticleDirection = FRotator(0.0f, FMath::RandRange(0, 360), 0.0f);

			UNiagaraComponent* NiagaraComp = UNiagaraFunctionLibrary::SpawnSystemAttached(SwordParticles, SelfActor->GetRootComponent(), NAME_None, FVector(0, 0, 100), ParticleDirection, EAttachLocation::Type::KeepRelativeOffset, true, true);
		}
		

		// Get the game mode
		if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
		{
			// Cast it to our specific game mode
			if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
			{
				FStringFormatNamedArguments MessageArgs;
				MessageArgs.Add("a", FString::Printf(TEXT("%.2f"), DamageDealt));

				// Send a basic message
				FString NewMessage(FString::Format(TEXT("I did {a} damage"), MessageArgs));
				AssignmentGameMode->SendCharacterMessage(NewMessage);
			}
		}


	}
	
}


void APlayerCharacter::Quit()
{
	if (UWorld* World = GetWorld()) {
		World->GetFirstPlayerController()->ConsoleCommand("quit");
	}
}


float APlayerCharacter::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (CanBeDamaged()) {
		PlayerHealth -= Damage;

		if (PlayerHealth <= 0) {
			SetCanBeDamaged(false);

			// Prevent sword damage
			SwordMesh->OnComponentBeginOverlap.Clear();

			Ragdoll();

			// Get the game mode
			if (AGameModeBase* GameMode = UGameplayStatics::GetGameMode(this))
			{
				// Cast it to our specific game mode
				if (AAssignment2GameMode* AssignmentGameMode = Cast<AAssignment2GameMode>(GameMode))
				{
					// Send death message
					AssignmentGameMode->SendCharacterMessage(TEXT("I have Died!"));
				}
			}


			// Start quit timer
			if (UGameInstance* GameInstance = GetGameInstance())
			{
				FTimerManager& QuitTimerManager = GameInstance->GetTimerManager();

				FTimerDelegate QuitTimerDelegate;

				QuitTimerDelegate.BindUObject(this, &APlayerCharacter::Quit);


				QuitTimerManager.SetTimer(QuitHandle, QuitTimerDelegate, 5.0f, true);
			}


			return Damage + PlayerHealth;
		}
		else {
			return Damage;
		}
	}
	return 0.0f;
}