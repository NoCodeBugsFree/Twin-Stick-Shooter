// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "TwinStickGameMode.generated.h"

/**
 * 
 */
UCLASS()
class TWINSTICKSHOOTER_API ATwinStickGameMode : public AGameMode
{
	GENERATED_BODY()

public:

	virtual void BeginPlay() override;
	
	/** Store next character respawn transform  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetCharacterNextSpawnTransform(FTransform NewTransform);

	/** Adds score to character's score   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void AddScore(int32 DeltaScore) { Score += DeltaScore; }

	/** Respawn Player  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void RespawnPlayer();

	/**  Respawn Player WithDelay */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void RespawnPlayerWithDelay(float RespawnPlayerDelay);

	/** Activate all Spawn Volumes  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void ActivateSpawnVolumes();

private:
	
	FTimerHandle RespawnPlayerTimer;

	/** Twin Stick Character Type Template to spawn  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATwinStickCharacter> TwinStickCharacterType;

	/** Spawn Delay after character's dead */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float SpawnDelay = 5.f;

	/** Spawn Timer  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	FTimerHandle SpawnTimer;
	
	/** Total Character's Score  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 Score = 0;
	
	/**   */
	FTransform CharacterNextSpawnTransform;
	
};
