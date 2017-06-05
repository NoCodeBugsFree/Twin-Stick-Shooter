// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	/** instant fire  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Fire();

	/** weapon try to fire  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StartFire();
	
	/** enable weapon to fire again if we have ammo */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void Reload();

	/** returns current ammo  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	int32 GetCurrentAmmo() const { return CurrentAmmo; }

protected:
	
	/** Can Weapon fire now or not   */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (BlueprintProtected = "true"))
	uint32 bCanFire : 1;

	/** Weapon's current ammo amount   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (BlueprintProtected = "true"))
	int32 CurrentAmmo = 100;

	/** Projectile template  */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (BlueprintProtected = "true"))
	TSubclassOf<class AProjectile> ProjectileType;

	/** Weapon reload time   */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "AAA", meta = (BlueprintProtected = "true"))
	float ReloadTime = 0.2f;

	FTimerHandle FireTimer;

private:

	/** Root Scene Component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USceneComponent* Scene;

	/** Weapon Skeletal Mesh  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* GunSkeletalMesh;

	/** Place in 3D world where Projectile Spawns */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UArrowComponent* ProjectileSpawnLocation;

	
};
