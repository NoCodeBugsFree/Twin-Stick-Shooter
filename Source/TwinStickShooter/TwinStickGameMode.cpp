// Fill out your copyright notice in the Description page of Project Settings.

#include "TwinStickGameMode.h"
#include "SpawnVolume.h"
#include "Kismet/GameplayStatics.h"
#include "TwinStickCharacter.h"

void ATwinStickGameMode::BeginPlay()
{
	Super::BeginPlay();

	// activate spawn volumes
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &ATwinStickGameMode::ActivateSpawnVolumes, SpawnDelay, false);
}

void ATwinStickGameMode::SetCharacterNextSpawnTransform(FTransform NewTransform)
{
	CharacterNextSpawnTransform = NewTransform;
}

void ATwinStickGameMode::RespawnPlayer()
{
	if (TwinStickCharacterType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			ATwinStickCharacter* TwinStickCharacter = World->SpawnActor<ATwinStickCharacter>(TwinStickCharacterType, CharacterNextSpawnTransform, SpawnParams);
			if (TwinStickCharacter)
			{
				GetWorld()->GetFirstPlayerController()->Possess(TwinStickCharacter);
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("set up TwinStickCharacterType"));
	}
}

void ATwinStickGameMode::RespawnPlayerWithDelay(float RespawnPlayerDelay)
{
	GetWorldTimerManager().SetTimer(RespawnPlayerTimer, this, &ATwinStickGameMode::RespawnPlayer, RespawnPlayerDelay, false);
}

void ATwinStickGameMode::ActivateSpawnVolumes()
{
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASpawnVolume::StaticClass(), FoundActors);
	
	for (auto Actor : FoundActors)
	{
		ASpawnVolume* TestActor = Cast<ASpawnVolume>(Actor);
		if (TestActor)
		{
			TestActor->SetShouldSpawn(true);
		}
	}
}
