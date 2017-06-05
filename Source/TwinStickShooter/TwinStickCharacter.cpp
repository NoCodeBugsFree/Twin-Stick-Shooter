// Fill out your copyright notice in the Description page of Project Settings.

#include "TwinStickCharacter.h"
#include "TwinStickShooter.h"
#include "Kismet/KismetMathLibrary.h"
#include "Weapon.h"
#include "TwinStickGameMode.h"
#include "Components/WidgetComponent.h"
#include "Components/ProgressBar.h"

#if WITH_EDITOR
void ATwinStickCharacter::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	/** Calculate bIsDead property at runtime editor property changing  */

	CalculateDead();

	Super::PostEditChangeProperty(PropertyChangedEvent);

}
#endif

// Sets default values
ATwinStickCharacter::ATwinStickCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false; // axis events ticks them selfs

	/** Selfy stick  */
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->bInheritPitch = false;
	CameraBoom->bInheritRoll = false;
	CameraBoom->bInheritYaw = false;
	CameraBoom->bEnableCameraLag = true;
	CameraBoom->bEnableCameraRotationLag = true;
	CameraBoom->CameraLagSpeed = 20.f;
	CameraBoom->CameraRotationLagSpeed = 20.f;
	CameraBoom->TargetArmLength = 1000.f;
	CameraBoom->bDoCollisionTest = false;

	/** Top Down Camera   */
	TopDownCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("TopDownCamera"));
	TopDownCamera->SetupAttachment(CameraBoom);

	// HealthBarWidget 
	HealthBarWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("HealthBarWidget"));
	HealthBarWidget->SetupAttachment(GetMesh());
	HealthBarWidget->SetWidgetSpace(EWidgetSpace::Screen);
	HealthBarWidget->SetDrawSize(FVector2D(50, 10));
	HealthBarWidget->SetRelativeLocation(FVector(0.f, 0.f, 200.f));
}

// Called when the game starts or when spawned
void ATwinStickCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Update HealthWidget which was hooked in Super::BeginPlay() - BP_HeroCharacter 
	UpdateHealthWidget();

	// Spawn a Gun if Player
	if ( !ActorHasTag("AI") )
	{
		SpawnWeapon();

		// set next spawn point
		SetNextSpawnTransform();

		// add GC function dynamic delegate
		OnDestroyed.AddDynamic(this, &ATwinStickCharacter::OnTwinStickCharacterDestroyed);
	}	
}

void ATwinStickCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	CameraBoom->SetRelativeRotation(FRotator(-70, 0, 0));
}

// Called every frame
void ATwinStickCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void ATwinStickCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
	/** Character Movement  */
	PlayerInputComponent->BindAxis("MoveUp", this, &ATwinStickCharacter::MoveUp);
	PlayerInputComponent->BindAxis("MoveRight", this, &ATwinStickCharacter::MoveRight);

	/** Character Rotation (and firing)  */
	PlayerInputComponent->BindAxis("LookUp", this, &ATwinStickCharacter::LookUp);
	PlayerInputComponent->BindAxis("LookRight", this, &ATwinStickCharacter::LookRight);
}

void ATwinStickCharacter::OnTwinStickCharacterDestroyed(AActor* DestroyedActor)
{
	if (CurrentWeapon)
	{
		CurrentWeapon->Destroy();
	}
}

void ATwinStickCharacter::SpawnWeapon()
{
	if (WeaponType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
		
			FTransform Transform = GetMesh()->GetComponentTransform();
			
			AWeapon* SpawnedWeapon = World->SpawnActor<AWeapon>(WeaponType, Transform, SpawnParams);
			if (SpawnedWeapon)
			{
				CurrentWeapon = SpawnedWeapon;
				CurrentWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale, "GunSocket");
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Set up WeaponType in BP_HeroCharacter"));
	}
}

void ATwinStickCharacter::CalculateDead()
{
	if (Health <= 0)
	{
		bIsDead = true;
	}
	else
	{
		bIsDead = false;
	}
}

void ATwinStickCharacter::CalculateHealth(float Delta)
{
	Health += Delta;
	CalculateDead();
}

void ATwinStickCharacter::SetProgressBarReference(UProgressBar* ProgressBarToSet)
{
	ProgressBarReference = ProgressBarToSet;
	UpdateHealthWidget();
}

void ATwinStickCharacter::UpdateHealthWidget()
{
	if (ProgressBarReference)
	{
		float LifePercent = Health / 100.f;

		ProgressBarReference->SetPercent(LifePercent);

		/** Color creation - from green to red  */
		FLinearColor Color(1 - LifePercent + 0.2, LifePercent - 0.2, 0);

		ProgressBarReference->SetFillColorAndOpacity(Color);
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ProgressBarReference = NULL in %s"), *GetName());
	}
}

void ATwinStickCharacter::StartFire()
{
	if (CurrentWeapon)
	{
		CurrentWeapon->StartFire();
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("CurrentWeapon == NULL "));
	}
}

bool ATwinStickCharacter::ShouldFire(FRotator CurrentRotation, FRotator TargetRotation)
{
	return CurrentRotation.Equals(TargetRotation);
}

float ATwinStickCharacter::TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	const float ActualDamage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	/** handle absorbed damage   */
	if (ActualDamage > 0.f)
	{
		CalculateHealth(-ActualDamage);

		if (bIsDead)
		{	
			Dead();
		}
	}

	UpdateHealthWidget();

	return ActualDamage;
}

void  ATwinStickCharacter::SpawnExplosion()
{
	SetLifeSpan(0.1);
	if (ExplosionType)
	{
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;

			FTransform Transform = GetMesh()->GetSocketTransform("Explosion");

			World->SpawnActor<AActor>(ExplosionType, Transform, SpawnParams);
		}
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Set up ExplosionType"));
	}
}

int32 ATwinStickCharacter::GetCurrentAmmo() const
{
	if (CurrentWeapon)
	{
		return CurrentWeapon->GetCurrentAmmo();
	}
	return 0;
}

void ATwinStickCharacter::SetNextSpawnTransform()
{
	ATwinStickGameMode* TwinStickGameMode = Cast<ATwinStickGameMode>(GetWorld()->GetAuthGameMode());
	if (TwinStickGameMode)
	{
		TwinStickGameMode->SetCharacterNextSpawnTransform(GetActorTransform());
		TwinStickGameModeReference = TwinStickGameMode;
	}
}

void ATwinStickCharacter::Dead_Implementation()
{
	DisableInput(Cast<APlayerController>(GetController()));
	SetActorEnableCollision(false);

	/** Hide progress bar widget   */
	if (ProgressBarReference)
	{
		ProgressBarReference->SetVisibility(ESlateVisibility::Hidden);
	}

	// FX
	GetWorldTimerManager().SetTimer(ExplosionTimer, this, &ATwinStickCharacter::SpawnExplosion, 2, false);

	// respawn in GameMode
	Respawn();
}

void ATwinStickCharacter::Respawn()
{
	if (TwinStickGameModeReference)
	{
		TwinStickGameModeReference->RespawnPlayerWithDelay(5.f);
	} 
	else
	{
		UE_LOG(LogTemp, Error, TEXT("ATwinStickCharacter::Respawn() - TwinStickGameModeReference == NULL"));
	}
}

void ATwinStickCharacter::MoveUp(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		FVector UpDirection = FVector(1, 0, 0);
		AddMovementInput(UpDirection, Value);
	}
}

void ATwinStickCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0))
	{
		FVector RightDirection = FVector(0, 1, 0);
		AddMovementInput(RightDirection, Value);
	}
}

void ATwinStickCharacter::LookUp(float Value)
{
	XValue = Value;
	RotateCharacter(Value);
}

void ATwinStickCharacter::LookRight(float Value)
{
	YValue = Value;
	RotateCharacter(Value);
}

void ATwinStickCharacter::RotateCharacter(float Value)
{
	if (Controller)
	{
		/** make vector from x and y , z = 0  */
		FVector XYVector = UKismetMathLibrary::MakeVector(XValue, YValue, 0.f);

		/** make desired rotator from this vector  */
		FRotator RotatorFromXVector = UKismetMathLibrary::MakeRotFromX(XYVector);
		
		if (XYVector.Size() > 0.25f)
		{
			FRotator CurrentRotation = Controller->GetControlRotation();

			/** if not ShouldFire - first rotate character in desired direction, and fire after if  */
			if (ShouldFire(CurrentRotation, RotatorFromXVector))
			{
				if (!FMath::IsNearlyZero(Value, 0.01f))
				{
					StartFire();
				}
			}
			else
			{
				/** Rotate in desired direction  */
				Controller->SetControlRotation(RotatorFromXVector);
			}
		}
	}
}

