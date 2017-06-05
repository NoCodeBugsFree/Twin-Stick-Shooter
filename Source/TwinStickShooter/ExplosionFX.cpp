// Fill out your copyright notice in the Description page of Project Settings.

#include "ExplosionFX.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicsEngine/RadialForceComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AExplosionFX::AExplosionFX()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	ExplosionParticles = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ExplosionParticles"));
	ExplosionParticles->SetupAttachment(RootComponent);
	ExplosionParticles->bAutoActivate = true;
	

	ExplosionRadialForce = CreateDefaultSubobject<URadialForceComponent>(TEXT("ExplosionRadialForce"));
	ExplosionRadialForce->SetupAttachment(RootComponent);
	ExplosionRadialForce->Radius = 500.f;
	ExplosionRadialForce->ImpulseStrength = 100000.f;
	ExplosionRadialForce->ForceStrength = 100000.f;
}

// Called when the game starts or when spawned
void AExplosionFX::BeginPlay()
{
	Super::BeginPlay();
	
	if (ExplosionSound)
	{
		UGameplayStatics::PlaySoundAtLocation(this, ExplosionSound, GetActorLocation());
	}
	ExplosionRadialForce->Activate(false);
	SetLifeSpan(2.f);
	
}

// Called every frame
void AExplosionFX::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

