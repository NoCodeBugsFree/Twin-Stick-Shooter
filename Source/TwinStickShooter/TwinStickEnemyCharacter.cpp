// Fill out your copyright notice in the Description page of Project Settings.

#include "TwinStickEnemyCharacter.h"
#include "TwinStickGameMode.h"
#include "Components/BoxComponent.h"
#include "TwinStickCharacter.h"
#include "Components/ProgressBar.h"


ATwinStickEnemyCharacter::ATwinStickEnemyCharacter()
{
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	DamageBox = CreateDefaultSubobject<UBoxComponent>(TEXT("DamageBox"));
	DamageBox->SetupAttachment(RootComponent);

}

void ATwinStickEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	DamageBox->OnComponentBeginOverlap.AddDynamic(this, &ATwinStickEnemyCharacter::OnDamageBoxOverlapBegin);
	DamageBox->OnComponentEndOverlap.AddDynamic(this, &ATwinStickEnemyCharacter::OnDamageBoxOverlapEnd);
}

void  ATwinStickEnemyCharacter::OnDamageBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ATwinStickCharacter* TwinStickCharacter = Cast<ATwinStickCharacter>(OtherActor);
		if (TwinStickCharacter && TwinStickCharacter->ActorHasTag("Friendly"))
		{
			CharacterToAttack = TwinStickCharacter;

			// start inflict damage to character
			GetWorldTimerManager().SetTimer(DamageTimer, this, &ATwinStickEnemyCharacter::InflictDamageTo, DamageCooldown, true, 0.2f);
		}
	}
}

void ATwinStickEnemyCharacter::OnDamageBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself. 
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ATwinStickCharacter* TwinStickCharacter = Cast<ATwinStickCharacter>(OtherActor);
		if (TwinStickCharacter && TwinStickCharacter->ActorHasTag("Friendly"))
		{
			// stop inflict damage to character
			GetWorldTimerManager().ClearTimer(DamageTimer);
		}
	}
}

void ATwinStickEnemyCharacter::InflictDamageTo()
{
	if (CharacterToAttack)
	{
		CharacterToAttack->TakeDamage(DamageAmount, FDamageEvent(), GetInstigatorController(), this);
	}
}

void ATwinStickEnemyCharacter::Dead_Implementation()
{
	DetachFromControllerPendingDestroy();
	SetActorEnableCollision(false);

	// hide health bar
	if (ProgressBarReference)
	{
		ProgressBarReference->SetVisibility(ESlateVisibility::Hidden);
	}

	// FX
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATwinStickCharacter::SpawnExplosion, 2, false);

	// Add Character's score
	ATwinStickGameMode* TwinStickGameMode = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode());
	if (TwinStickGameMode)
	{
		TwinStickGameMode->AddScore(ScoreReward);
	}
}
