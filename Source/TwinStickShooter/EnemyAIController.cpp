// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIController.h"

void AEnemyAIController::BeginPlay()
{
	Super::BeginPlay();

	GetWorldTimerManager().SetTimer(TrackPlayerTimer, this, &AEnemyAIController::TrackPlayer, 1.f, true);

}

void AEnemyAIController::TrackPlayer()
{
	FAIMoveRequest AIMoveRequest;
	AIMoveRequest.SetGoalActor(GetWorld()->GetFirstPlayerController()->GetPawn());
	MoveTo(AIMoveRequest);
}
