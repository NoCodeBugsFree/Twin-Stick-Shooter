// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "EnemyAIController.generated.h"

/**
 * 
 */
UCLASS()
class TWINSTICKSHOOTER_API AEnemyAIController : public AAIController
{
	GENERATED_BODY()
	
public:

	virtual void BeginPlay() override;
	
protected:

	/** Simple Track Player Character  */
	void TrackPlayer();

private:
	
	/** Timer to handle Track Player*/
	FTimerHandle TrackPlayerTimer;
	
};
