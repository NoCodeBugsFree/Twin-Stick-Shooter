// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TwinStickCharacter.generated.h"

class AWeapon;

UCLASS()
class TWINSTICKSHOOTER_API ATwinStickCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ATwinStickCharacter();

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Construction  */
	virtual void OnConstruction(const FTransform& Transform) override;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/** GC function - helps destroy player's weapon   */
	UFUNCTION()
	void OnTwinStickCharacterDestroyed(AActor* DestroyedActor);

	/** Spawn a weapon   */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnWeapon();

	/** Returns current Character's Health  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	float GetHealt() const { return Health; }

	/** Returns current IsDead state  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	bool GetIsDead() const { return bIsDead; }

	// helper
	virtual void CalculateDead();

	/** Calculate Character's health and set IsDead state accordingly */
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void CalculateHealth(float Delta);

	// Editor code to make updating values in the editor cleaner
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

protected:

	/** Character's Health  */
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "AAA", meta = (BlueprintProtected = "true"))
	float Health = 100;

	/** Is Character alive now or not */
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "AAA")
	bool bIsDead = false;

	UPROPERTY()
	FTimerHandle ExplosionTimer;

	UPROPERTY()
	FTimerHandle RespawnTimer;

	/** GameMode Reference  */
	UPROPERTY()
	class ATwinStickGameMode* TwinStickGameModeReference;

	/** Health Bar Widget Reference  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UWidgetComponent* HealthBarWidget;

	/** Progress Bar Reference for update */
	UPROPERTY()
	class UProgressBar* ProgressBarReference;
	
public:

	/** Sets the Progress Bar percentage  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetProgressBarReference(UProgressBar* ProgressBarToSet);

	/** Update Health Widget - calls when health updates  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void UpdateHealthWidget();
	
	/** Try to fire, delegates to Weapon function fire  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void StartFire();

	/** to prevent fire wrong direction  */
	bool ShouldFire(FRotator CurrentRotation, FRotator TargetRotation);

	/** Take Damage handle */
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	/** Explosion FX when die  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SpawnExplosion();

	/** Returns current ammo in current weapon if weapon is valid, else returns 0  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	int32 GetCurrentAmmo() const;

	/** Set character next spawn point in GameMode  */
	UFUNCTION(BlueprintCallable, Category = "AAA")
	void SetNextSpawnTransform();

	/** Dead NativeEvent   */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "AAA")
	void Dead();
	virtual void Dead_Implementation();

	/** Returns CameraBoom subobject  */
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }

	/** Returns Camera subobject  */
	FORCEINLINE class UCameraComponent* GetCamera() const { return TopDownCamera; }
	
private:

	/** GameMode respawn character after death  */
	void Respawn();
	
	/** Explosion Type Template */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AActor> ExplosionType;

	/** Weapon Type Template */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<class AWeapon> WeaponType;

	/** Current tWeapon Reference */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	AWeapon* CurrentWeapon;

	/** Look up value to store   */
	float XValue = 0.f;
	
	/** Look right value to store  */
	float YValue = 0.f;

	/** Move the character in up/down direction  */
	void MoveUp(float Value);

	/** Move the character in left/right direction  */
	void MoveRight(float Value);

	/** Move the character in up/down direction  */
	void LookUp(float Value);

	/** Move the character in left/right direction  */
	void LookRight(float Value);

	/** Move the character in left/right direction  */
	void RotateCharacter(float Value);

	/** CameraBoom subobject  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class USpringArmComponent* CameraBoom;

	/** TopDownCamera subobject  */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AAA", meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* TopDownCamera;
	
};
