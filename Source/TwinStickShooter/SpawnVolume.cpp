// Fill out your copyright notice in the Description page of Project Settings.

#include "SpawnVolume.h"
#include "TwinStickEnemyCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ASpawnVolume::ASpawnVolume()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	SpawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	SpawnArea->SetupAttachment(RootComponent);
	SpawnArea->bGenerateOverlapEvents = false;
	SpawnArea->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	/** Game mode sets the game rules  */
	bShouldSpawn = false;
}

// Called when the game starts or when spawned
void ASpawnVolume::BeginPlay()
{
	Super::BeginPlay();
	
	/** initialize the timer only, Game mode sets when start spawning */
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ASpawnVolume::SpawnEnemy, SpawnDelay, true);
}

// Called every frame
void ASpawnVolume::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpawnVolume::SpawnEnemy()
{
	if ( (bShouldSpawn) && ( !IsMaximumNumberOfEnemiesReached()) )
	{
		if (EnemyTypeToSpawn)
		{
			UWorld* const World = GetWorld();
			if (World)
			{
				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = Instigator;

				FVector Origin = SpawnArea->Bounds.Origin;
				FVector Extent = SpawnArea->Bounds.BoxExtent;

				FVector SpawnLocation = UKismetMathLibrary::RandomPointInBoundingBox(Origin, Extent);
				FRotator SpawnRotation = FRotator::ZeroRotator;
				
				World->SpawnActor<ATwinStickEnemyCharacter>(EnemyTypeToSpawn, SpawnLocation, SpawnRotation, SpawnParams);
			}
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("set up EnemyTypeToSpawn"));
		}
	}
}

bool ASpawnVolume::IsMaximumNumberOfEnemiesReached() const
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ATwinStickEnemyCharacter::StaticClass(), FoundActors);
	
	return FoundActors.Num() == MaximumNumberOfEnemies;
}
