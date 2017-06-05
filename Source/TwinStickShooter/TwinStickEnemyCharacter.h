// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "TwinStickCharacter.h"
#include "TwinStickEnemyCharacter.generated.h"

/**
 * 
 */
UCLASS()
class TWINSTICKSHOOTER_API ATwinStickEnemyCharacter : public ATwinStickCharacter
{
	GENERATED_BODY()
	
public:

	ATwinStickEnemyCharacter();

	virtual void BeginPlay() override;

	/** Dead Native Event override   */
	virtual void Dead_Implementation() override;

	/** Start Damage Player  */
	UFUNCTION()
	void OnDamageBoxOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	/** Stop Damage Player  */
	UFUNCTION()
	void OnDamageBoxOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);
	
private:

	/** Prey reference to store  */
	UPROPERTY()
	class ATwinStickCharacter* CharacterToAttack;

	/** Inflict Damage To target  */
	void InflictDamageTo();

	FTimerHandle DamageTimer;

	/** Damage Amount to apply per damage cooldowns */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float DamageAmount = 20.f;
	
	/** delay between attacks  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float DamageCooldown = 1.f;

	/** Damage Box area */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* DamageBox;
	
	/** Score Reward for character to kill me */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 ScoreReward = 10;
	
};
