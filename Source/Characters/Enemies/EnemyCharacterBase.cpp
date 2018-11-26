// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyCharacterBase.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Animations/EnemyAnimationBasse.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Controllers/EnemyAIControllerBase.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LevelActors/RoomController.h"

// Sets default values        
AEnemyCharacterBase::AEnemyCharacterBase() : ABattleCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	HealthBarComponent = CreateDefaultSubobject<UWidgetComponent>(FName("Heath Bar"));
	HealthBarComponent->SetupAttachment(RootComponent);

	VoicesComponent = CreateDefaultSubobject<UActionVoicesComponent>(TEXT("Action Voices Component"));
	
}

// Called when the game starts or when spawned
void AEnemyCharacterBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AEnemyCharacterBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TWeakObjectPtr<APawn> pawn = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();

	if (pawn.IsValid())
	{
		FVector CameraLocation = GetWorld()->GetFirstPlayerController()->PlayerCameraManager->GetCameraLocation();

		FRotator LookAtRot = FRotationMatrix::MakeFromX(CameraLocation - HealthBarComponent->GetComponentLocation()).Rotator();

		LookAtRot.Pitch = 0.0f;
		LookAtRot.Roll = 0.0f;

		HealthBarComponent->SetWorldRotation(LookAtRot);
		if (IsAlive) UpdateHealthbar();
	}
	
}

// Called to bind functionality to input
void AEnemyCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ABattleCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemyCharacterBase::PostInitializeComponents()
{
	ABattleCharacterBase::PostInitializeComponents();
	
	BodyCollision->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator(90.0f, 00.0f, 0.0f));
	BodyCollision->OnComponentBeginOverlap.AddDynamic(this, &AEnemyCharacterBase::OnBeginBodyOverlap);

	HealthBarComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);

	FVector location = HealthBarComponent->GetRelativeTransform().GetLocation();
	FRotator rotation = HealthBarComponent->GetRelativeTransform().GetRotation().Rotator();

	location.Z = 160.0f;
	rotation.Yaw = 0.0f;

	HealthBarComponent->SetRelativeLocationAndRotation(location, rotation);

	if (IsValid(HealthBarWidgetClass)) {
		HealthBarComponent->SetWidgetClass(HealthBarWidgetClass);
	}

	if (IsValid(CombatComp))
	{
		CombatComp->InitializeCombatComponent(CharacterName, CurrentLevel, EquipmentManager, VoicesComponent, this);
		UpdateHealthbar();

		USkeletalMeshComponent* RootMesh = GetMesh();
		UCombatData* CombatData = CombatComp->GetCharacterCombatData();

		if (IsValid(CombatData))
		{
			if (IsValid(RightWeaponClass))
			{

				FEquipmentData RightWeapon;
				RightWeapon.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
				RightWeapon.WeaponMeshComp = nullptr;
				RightWeapon.WeaponClass = RightWeaponClass;
				RightWeapon.BattleCharacter = this;
				RightWeapon.bCreateMeshComponent = true;
				RightWeapon.RootMeshComp = RootMesh;

				//equip this weapon on equipment comp
				EquipmentManager->EquipRightHand(&RightWeapon, CombatData);

				if (CombatComp->GetCharacterCombatData()->GetIsDualWeilding() && IsValid(LeftWeaponClass))
				{
					FEquipmentData LeftWeapon;
					LeftWeapon.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
					LeftWeapon.WeaponMeshComp = nullptr;
					LeftWeapon.WeaponClass = LeftWeaponClass;
					LeftWeapon.BattleCharacter = this;
					LeftWeapon.bCreateMeshComponent = true;
					LeftWeapon.RootMeshComp = RootMesh;
					//equip this weapon on equipment comp
					EquipmentManager->EquipLeftHand(&LeftWeapon, CombatData);

				}
			}

			if (ArmorClasses.Num() > 0)
			{
				for (const TPair<ArmorSlotTypes, TSubclassOf<UArmorObject>>& Pairs : ArmorClasses)
				{
					FEquipmentData Armor;
					Armor.bCreateMeshComponent = true;
					Armor.ArmorClass = Pairs.Value;
					Armor.ArmorSlot = Pairs.Key;
					Armor.RootMeshComp = RootMesh;
					Armor.Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
					Armor.WeaponClass = nullptr;
					Armor.ArmorMeshComp = nullptr;

					EquipmentManager->EquipArmorSlot(&Armor, CombatData);
				}
			}
		}
	}
}


void AEnemyCharacterBase::PossessedBy(AController * NewController)
{
	ABattleCharacterBase::PossessedBy(NewController);
	AIController = Cast<AEnemyAIControllerBase>(NewController);

	if (IsValid(AIController))
	{
		UpdateHealthbar();
	}
}

void AEnemyCharacterBase::UnPossessed()
{
}

void AEnemyCharacterBase::Landed(const FHitResult & Hit)
{
}

void AEnemyCharacterBase::MoveBlockedBy(const FHitResult & Impact)
{
    //UE_LOG(LogTemp, Warning, TEXT("Movement Blocked!!"));

    if (AIController && AIController->IsValidLowLevel()) {
        if (AIController->GetAIConfig()->IsChasing)
        {
            if (Impact.bBlockingHit)
            {
                TWeakObjectPtr<APlayerCharacterBase> Player = Cast<APlayerCharacterBase>(Impact.GetActor());
                TWeakObjectPtr<AEnemyCharacterBase> Enemy = Cast<AEnemyCharacterBase>(Impact.GetActor());
                if (Player.IsValid())
                {
                    //UE_LOG(LogTemp, Warning, TEXT("Blocked By Player...Aborting movement."));
                    AIController->AbortMoveTo();
                }
                else if (Enemy.IsValid()) 
                {
                   // UE_LOG(LogTemp, Warning, TEXT("Blocked By Another Enemy, rerouting movement."));
                    AIController->RerouteChasePlayer();
                }
            }
        }
        else if (AIController->GetAIConfig()->IsMoving)
        {
            if (Impact.bBlockingHit)
            {
                TWeakObjectPtr<APlayerCharacterBase> Player = Cast<APlayerCharacterBase>(Impact.GetActor());
                TWeakObjectPtr<AEnemyCharacterBase> Enemy = Cast<AEnemyCharacterBase>(Impact.GetActor());
                if (Player.IsValid())
                {
                    //UE_LOG(LogTemp, Warning, TEXT("Blocked By Player...Notify Perception!"));
                    AIController->AbortMoveTo();
                    //report no damage, but, this should trigger perception
                    AIController->AlertDamageEvent(Player.Get(), 0.0f);
                }
                else if (Enemy.IsValid())
                {
                    //UE_LOG(LogTemp, Warning, TEXT("Blocked By Another Enemy, lets stop."));
                    AIController->AbortMoveTo();
                }
            }
        }
        
    }
}

void AEnemyCharacterBase::OnNotifyAttackComplete()
{
	ABattleCharacterBase::OnNotifyAttackComplete();
    //UE_LOG(LogTemp, Warning, TEXT("Attack Completed!"));
	
	EnableWeaponCollider(ECollisionEnabled::NoCollision);
}

bool AEnemyCharacterBase::ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult)
{
	if (IsAlive) {
		CombatComp->ComputeCombatResult(InDamageData, OutResult);

		DamageResultList.Add(OutResult);

		SpawnFloatingText();

		if (!OutResult.IsStillAlive)
		{
			Kill();
		}

		
		if (OutResult.HitSuccess) {
			//UE_LOG(LogTemp, Warning, TEXT("Receiving Damage!"));
		
			AIController->AlertDamageEvent(this, 10.0f);
			UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

			if (IsValid(AnimInst)) {
				UEnemyAnimationBasse* Animation = Cast<UEnemyAnimationBasse>(AnimInst);

				if (IsValid(Animation)) {
					Animation->PlayTakeHitAnimation();
				}
			}
		}
		UpdateHealthbar();
	}

	return OutResult.HitSuccess;
}

void AEnemyCharacterBase::OnBeginBodyOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	if (IsValid(OtherActor)) {
		APlayerCharacterBase* Player = Cast<APlayerCharacterBase>(OtherActor);

		if (IsValid(Player)) {
          
		}
	}
}

bool AEnemyCharacterBase::PerformAreaAttack(const FName& AnimSectionName, bool bIsRageAttack)
{
	return ABattleCharacterBase::PerformAreaAttack(AnimSectionName, bIsRageAttack);
}

bool AEnemyCharacterBase::PerformSingleAttack(const FName& AnimSectionName)
{
	IsAttacking = false;

	if (ABattleCharacterBase::PerformSingleAttack(AnimSectionName))
	{
		TWeakObjectPtr<UAnimInstance> AnimInst = GetMesh()->GetAnimInstance();
		TWeakObjectPtr<UEnemyAnimationBasse> EnemyAnim = Cast<UEnemyAnimationBasse>(AnimInst);

		if (EnemyAnim.IsValid() && EnemyAnim->IsValidLowLevel()) 
		{
			EnemyAnim->PlayAnimationSection(AnimSectionName);
			IsAttacking = true;
		}
		
	}

	return IsAttacking;
}

bool AEnemyCharacterBase::PerformRageAttack(const FName& AnimSectionName)
{
	return ABattleCharacterBase::PerformRageAttack(AnimSectionName);
}

void AEnemyCharacterBase::EnableWeaponCollider(ECollisionEnabled::Type Enable)
{
	if (IsUsingAreaAttack && Enable == ECollisionEnabled::QueryAndPhysics)
	{
		TArray<FHitResult> TargetsHit;
		const FVector StartLocation = GetActorLocation();
		const FVector EndLocation = StartLocation + (GetActorForwardVector() * FVector(50.0f, 0.0f, 0.0f));

		FCollisionShape Sphere = Sphere.MakeSphere(300.0f);

		GetWorld()->SweepMultiByProfile(TargetsHit, StartLocation, EndLocation,
			FQuat::Identity, FName("EnemyWeaponProfile"), Sphere);

		if (TargetsHit.Num() > 0)
		{
			//AIController->NotifyWeaponAreaHit(TargetsHit);
		}
	}
	else
	{
		EquipmentManager->SetWeaponCollisionEnabled(Enable);
	}
}

void AEnemyCharacterBase::Kill()
{
	if (IsAlive)
	{
		Super::Kill();

		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

		if (IsValid(AnimInst)) {
			UEnemyAnimationBasse* EnemyAnim = Cast<UEnemyAnimationBasse>(AnimInst);

			if (IsValid(EnemyAnim)) {
				EnemyAnim->PlayDeathAnimation();
			}
		}

		AController* PawnController = GetController();

		if (IsValid(PawnController)) {
			AIController = Cast<AEnemyAIControllerBase>(PawnController);
			if (IsValid(AIController)) {
				AIController->Kill();
			}
		}

		IsAlive = false;

		OwnerRoomController->NotifyEnemyDeath(OwnerMapKeyName);

	}
}



void AEnemyCharacterBase::TimedDestroyObject()
{
    Destroy();
}

void AEnemyCharacterBase::SetRoomController(const TWeakObjectPtr<ARoomController> owner, const FName& KeyName)
{
	OwnerMapKeyName = KeyName;

	OwnerRoomController = owner;
}

bool AEnemyCharacterBase::GetIsEnemyAlive()
{
	return IsAlive;
}

void AEnemyCharacterBase::SetEnableTargetLock(bool enable)
{
	if (IsValid(this))
	{
		SetHighlightedAsTarget(false);
		UEnemyHealthBarWidget* Healthbar = Cast<UEnemyHealthBarWidget>(HealthBarComponent->GetUserWidgetObject());
		if (IsValid(Healthbar)) {
			Healthbar->EnableHighlightText(enable);
		}
	}
}

void AEnemyCharacterBase::SetHighlightedAsTarget(bool enable)
{
	if (IsValid(this))
	{
		GetMesh()->SetRenderCustomDepth(enable);
		EquipmentManager->SetEnableHighlightAll(enable);
	}
}

void AEnemyCharacterBase::UpdateHealthbar()
{
	if (IsValid(AIController) && IsValid(CombatComp))
	{
		UEnemyHealthBarWidget* Healthbar = Cast<UEnemyHealthBarWidget>(HealthBarComponent->GetUserWidgetObject());
		if (IsValid(Healthbar)) {
			const UCombatData* BattleStats = CombatComp->GetCharacterCombatData();
			if (IsValid(BattleStats)) {
				Healthbar->UpdateInfo(BattleStats->GetCharacterName(), BattleStats->GetCurrentLevel(), BattleStats->GetHealthPercent());
			}
		}
	}
}

void AEnemyCharacterBase::PostActorCreated()
{
	ABattleCharacterBase::PostActorCreated();
}


void AEnemyCharacterBase::OnNotifyWeaponHitTarget(AActor* HitActor, const FVector& HitLocation, bool IsLeftWeapon)
{
	FCombatDamageResult Result;
	const UAttackAbility* SelectedAttackAbility = AIController->GetSelectedAbility();
	const UAttackAbility* SelectedRageAbility = Cast<URageAttackAbility>(SelectedAttackAbility);
	ABattleCharacterBase* CurrentTarget = AIController->GetSensedTarget();
	uint32 bUsingComboAbility = false;// AIController->GetIsUsingCombo();

	if (HitActor == CurrentTarget)
	{
		if (IsLeftWeapon)
		{
			TWeakObjectPtr<UWeaponObject> LeftWeaponObject = EquipmentManager->GetEquippedLeftWeapon()->GetWeaponObject();
			if (LeftWeaponObject.IsValid())
			{
				if (IsValid(SelectedAttackAbility))
				{
					CombatComp->ComputeWeaponDamage(CurrentTarget, LeftWeaponObject.Get(), SelectedAttackAbility, HitLocation, &Result, bUsingComboAbility);
				}
				else if (IsValid(SelectedRageAbility))
				{
					CombatComp->ComputeWeaponDamage(CurrentTarget, LeftWeaponObject.Get(), SelectedRageAbility, HitLocation, &Result, bUsingComboAbility);
				}
			}
		}
		else
		{
			TWeakObjectPtr<UWeaponObject> RightWeaponObject = EquipmentManager->GetEquippedRightWeapon()->GetWeaponObject();
			if (RightWeaponObject.IsValid())
			{
				if (IsValid(SelectedAttackAbility))
				{
					CombatComp->ComputeWeaponDamage(CurrentTarget, RightWeaponObject.Get(), SelectedAttackAbility, HitLocation, &Result, bUsingComboAbility);
				}
				else if (IsValid(SelectedRageAbility))
				{
					CombatComp->ComputeWeaponDamage(CurrentTarget, RightWeaponObject.Get(), SelectedRageAbility, HitLocation, &Result, bUsingComboAbility);
				}
			}
		}
		const FString CharName = CombatComp->GetCharacterCombatData()->GetCharacterName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s Attacks %s for %f damage!"), *CharName, *HitActor->GetName(), Result.DamageReceived);
	}
}

void AEnemyCharacterBase::SetEnemyLevel(const uint8 level)
{
	if (IsValid(CombatComp))
	{
		CombatComp->GetCharacterCombatData()->InitializeData(CharacterName, level);
	}
}

void AEnemyCharacterBase::EnableCombatMode(bool enable)
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInst)) {
		UEnemyAnimationBasse* EnemyAnim = Cast<UEnemyAnimationBasse>(AnimInst);

		if (IsValid(EnemyAnim)) {
			IsInCombat = enable;
			EnemyAnim->InBattle = enable;
		}
	}
}

void AEnemyCharacterBase::PlayChaseAnimation(bool play)
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInst)) {
		UEnemyAnimationBasse* EnemyAnim = Cast<UEnemyAnimationBasse>(AnimInst);

		if (IsValid(EnemyAnim)) {
			EnemyAnim->IsChasing = play;
		}
	}
}


void AEnemyCharacterBase::PlayMovingAnimation(bool play)
{
	UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();

	if (IsValid(AnimInst)) {
		UEnemyAnimationBasse* EnemyAnim = Cast<UEnemyAnimationBasse>(AnimInst);

		if (IsValid(EnemyAnim)) {
			EnemyAnim->IsMoving = play;
		}
	}
}
