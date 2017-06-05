// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpawnVolume.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API ASpawnVolume : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASpawnVolume();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/**   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnEnemy();

	/** Set bShouldSpawn state  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetShouldSpawn(bool NewhouldSpawn) { bShouldSpawn = NewhouldSpawn; }

	/** Need we spawn more enemies ?   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool IsMaximumNumberOfEnemiesReached() const;
	
private:
	
	FTimerHandle SpawnTimer;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	float SpawnDelay = 2.f;

	/**  Maximum Number Of Enemies which can be spawned by this spawn volume */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	int32 MaximumNumberOfEnemies = 3;

	/** Enemy Type To Spawn  */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class ATwinStickEnemyCharacter> EnemyTypeToSpawn;
	
	/** Should Spawn current state */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	uint32 bShouldSpawn : 1;

	/** Spawn area  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UBoxComponent* SpawnArea;
	
	
};
