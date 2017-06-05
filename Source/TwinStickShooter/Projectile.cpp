// Fill out your copyright notice in the Description page of Project Settings.

#include "Projectile.h"
#include "Components/PointLightComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystem.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	OnActorBeginOverlap.AddDynamic(this, &AProjectile::OnProjectileBeginOverlap);

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);
	
	SphereCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollision"));
	SphereCollision->SetupAttachment(RootComponent);
	SphereCollision->SetSphereRadius(10.f);

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetupAttachment(RootComponent);
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	LightComp = CreateDefaultSubobject<UPointLightComponent>(TEXT("LightComp"));
	LightComp->SetupAttachment(RootComponent);
	LightComp->SourceLength = 20.f;
	LightComp->AttenuationRadius = 400.f;
	LightComp->CastShadows = false;
	LightComp->SetLightColor(FLinearColor(0, 1, 0));

	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
	ProjectileMovementComponent->InitialSpeed = 3000.f;
	ProjectileMovementComponent->MaxSpeed = 3000.f;
	ProjectileMovementComponent->ProjectileGravityScale = 0.f;
	InitialLifeSpan = 3.33f;
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AProjectile::OnProjectileBeginOverlap(AActor* OverlappedActor, AActor* OtherActor)
{
	if (OtherActor && !OtherActor->ActorHasTag("Friendly"))
	{
		// inflict damage
		OtherActor->TakeDamage(LaserDamage, FDamageEvent(), GetInstigatorController(), this);

		// FX
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaserSparksTemplate, GetActorTransform());

		// TODO interaction sound

		SetLifeSpan(0.01f);
	}
}

