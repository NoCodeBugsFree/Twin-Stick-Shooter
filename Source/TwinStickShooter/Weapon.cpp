// Fill out your copyright notice in the Description page of Project Settings.

#include "Weapon.h"
#include "Components/ArrowComponent.h"
#include "Projectile.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	/** Scene Component  */
	Scene = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	SetRootComponent(Scene);

	/** Gun Skeletal Mesh  */
	GunSkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunSkeletalMesh"));
	GunSkeletalMesh->SetupAttachment(Scene);

	/** Projectile Spawn Location  */
	ProjectileSpawnLocation = CreateDefaultSubobject<UArrowComponent>(TEXT("ProjectileSpawnLocation"));
	ProjectileSpawnLocation->SetupAttachment(GunSkeletalMesh);
	
	bCanFire = true;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AWeapon::Fire()
{
	if (ProjectileType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
		
			FTransform ProjectileSpawnTransform = ProjectileSpawnLocation->GetComponentTransform();
			
			AProjectile* SpawnedProjectile = World->SpawnActor<AProjectile>(ProjectileType, ProjectileSpawnTransform, SpawnParams);
			if (SpawnedProjectile)
			{
				CurrentAmmo--;
				// TODO  SpawnFireEffect();
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Set up ProjectileType in BP_Weapon!"));
	}
}

void AWeapon::StartFire()
{
	if (CurrentAmmo > 0 && bCanFire)
	{
		/** temporary disable fire ability  */
		bCanFire = false;
		
		/** fire once  */
		Fire();

		/** enable fire ability in reload time  */
		GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::Reload, ReloadTime, false);
	}
}

void AWeapon::Reload()
{
	if (CurrentAmmo > 0)
	{
		bCanFire = true;
	}
}
