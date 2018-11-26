// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterBase.h"
#include "Characters/AnimNotifies/CharacterAnimNotify.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Controllers/PlayerControllerBase.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "../Enemies/EnemyCharacterBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISense_Damage.h"
#include "DrawDebugHelpers.h"
#include "Components/ActionVoicesComponent.h"
#include "../Enemies/LootItems/ExperienceDropActor.h"
#include "../Enemies/LootItems/ModifyStatsDrop.h"
#include "../Enemies/LootItems/UseableItemDrop.h"
#include "Runtime/Engine/Classes/GameFramework/CharacterMovementComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "LevelActors/DungeonDoorActor.h"


// Sets default values
APlayerCharacterBase::APlayerCharacterBase() : ABattleCharacterBase()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm"));
	SpringArmComponent->SetupAttachment(RootComponent);
	
	PlayerCamera = CreateDefaultSubobject<UCameraComponent>(FName("Camera"));
	PlayerCamera->SetupAttachment(SpringArmComponent, SpringArmComponent->GetAttachSocketName());

	VoicesComp = CreateDefaultSubobject<UActionVoicesComponent>(FName("Action Voices Comp"));
	AIPerceptionSource = CreateDefaultSubobject<UAIPerceptionStimuliSourceComponent>(FName("Perception Source"));
}

// Called when the game starts or when spawned
void APlayerCharacterBase::BeginPlay()
{
	ABattleCharacterBase::BeginPlay();
    
}

void APlayerCharacterBase::HandleItemPickup(const TWeakObjectPtr<ALootableItemBase> LootableItem)
{
	if (LootableItem.IsValid() && LootableItem->IsValidLowLevel())
	{
		UCombatData* BattleStats = CombatComp->GetCharacterCombatData();

		if (IsValid(LootableItem->PickupEffect)) {
			UParticleSystemComponent* Particle = UGameplayStatics::SpawnEmitterAttached(LootableItem->PickupEffect, RootComponent);
			Particle->bAutoDestroy = true;
		}
		if (IsValid(LootableItem->PickupSound)) {
			UGameplayStatics::PlaySoundAtLocation(this, LootableItem->PickupSound, GetActorLocation());
		}

		LootableItemTypes type = LootableItem->GetLootItemType();

		switch (type)
		{
		case LootableItemTypes::ExperienceDrop:
		{
			TWeakObjectPtr<AExperienceDropActor> ExpDrop = Cast<AExperienceDropActor>(LootableItem);

			if (ExpDrop.IsValid()) {

				if(IsValid(BattleStats)) {
					BattleStats->ModifyExp(ExpDrop->GetExpReward());
				}

			}
			break;
		}
		case LootableItemTypes::HealthReplenishDrop:
			if (IsValid(BattleStats)) {
				float health_modifier = BattleStats->GetExternalStats()->HealthPointsMax * LootableItem->DamageValue;
				BattleStats->ModifyHitPoints(health_modifier);
				PlayerController->UpdatePlayerUI();
			}
			break;

		case LootableItemTypes::StatsModifyDrop:
		{
			
			break;
		}
		case LootableItemTypes::CurrencyDrop:
			break;

		case LootableItemTypes::WeaponUpgradeDrop:
			break;

		case LootableItemTypes::ItemDrop:
		{
// 			TWeakObjectPtr<AUseableItemDrop> InventoryItem = Cast<AUseableItemDrop>(LootableItem.Get());
// 
// 			if (InventoryItem.IsValid())
// 			{
// 				PlayerController->AddLootableItemToInventory(InventoryItem->ItemName, InventoryItem);
// 			}
			break;
		}
		default:
			break;
		}

		
		LootableItem->Destroy();
	}
}

// Called every frame
void APlayerCharacterBase::Tick(float DeltaTime)
{
	ABattleCharacterBase::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void APlayerCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ABattleCharacterBase::SetupPlayerInputComponent(PlayerInputComponent);
    
}

void APlayerCharacterBase::PostInitializeComponents()
{
	ABattleCharacterBase::PostInitializeComponents();

	BodyCollision->SetRelativeLocationAndRotation(FVector::ZeroVector, FRotator(90.0f, 00.0f, 0.0f));
	BodyCollision->SetCapsuleHalfHeight(80.0f);
	BodyCollision->SetCapsuleRadius(60.0f);
	BodyCollision->OnComponentBeginOverlap.AddDynamic(this, &APlayerCharacterBase::OnBeginBodyOverlap);

	AIPerceptionSource->RegisterForSense(UAISense_Sight::StaticClass());
    AIPerceptionSource->RegisterForSense(UAISense_Damage::StaticClass());
	AIPerceptionSource->RegisterWithPerceptionSystem();

	CombatComp->InitializeCombatComponent(CharacterName, CurrentLevel, EquipmentManager, VoicesComp, this);

	EquipGear();

	CombatComp->InitializeCombatComponent(CharacterName, CurrentLevel, EquipmentManager, VoicesComp, this);

}

void APlayerCharacterBase::PossessedBy(AController * NewController)
{
	ABattleCharacterBase::PossessedBy(NewController);
    PlayerController = Cast<APlayerControllerBase>(NewController);
    
}


void APlayerCharacterBase::UnPossessed()
{
	ABattleCharacterBase::UnPossessed();
}

void APlayerCharacterBase::NotifyJumpApex()
{
	ABattleCharacterBase::NotifyJumpApex();

}

void APlayerCharacterBase::Landed(const FHitResult & Hit)
{
	ABattleCharacterBase::Landed(Hit);

}

void APlayerCharacterBase::MoveBlockedBy(const FHitResult & Impact)
{
	ABattleCharacterBase::MoveBlockedBy(Impact);

}

void APlayerCharacterBase::OnNotifyAttackComplete()
{
	ABattleCharacterBase::OnNotifyAttackComplete();

	if (IsValid(EquipmentManager))
	{
		EquipmentManager->SetWeaponCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}



void APlayerCharacterBase::MouseY(float value)
{
	if (IsValid(SpringArmComponent))
	{
		FRotator rotation = SpringArmComponent->GetRelativeTransform().Rotator();

		rotation.Pitch += value;

		rotation.Pitch = FMath::ClampAngle(rotation.Pitch, -45.0f, 45.0f);

		SpringArmComponent->SetRelativeRotation(rotation);
	}
}

bool APlayerCharacterBase::PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack)
{
	bool  success = false;

	
	success = ABattleCharacterBase::PerformAreaAttack(AnimSectionName, IsRageAttack);
	
	return success;
}

bool APlayerCharacterBase::PerformSingleAttack(const FName& AnimSectionName)
{
	bool success = false;

	
	success = ABattleCharacterBase::PerformSingleAttack(AnimSectionName);
	

	return success;
}

bool APlayerCharacterBase::PerformRageAttack(const FName& AnimSectionName)
{
	bool success = false;

    
	success = ABattleCharacterBase::PerformRageAttack(AnimSectionName);
    

	return success;
}

void APlayerCharacterBase::OnBeginBodyOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    if (IsValid(OtherActor)) {
        TWeakObjectPtr<AEnemyCharacterBase> Enemy = Cast<AEnemyCharacterBase>(OtherActor);
		TWeakObjectPtr<ALootableItemBase> item = Cast<ALootableItemBase>(OtherActor);
		
		if (Enemy.IsValid()) {
           
        }
        else if (item.IsValid())
		{
            HandleItemPickup(item);
        }
		
    }
}

// void APlayerCharacterBase::OnSingleAttackOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
// {
// 	if (IsValid(OtherActor) && PlayerController.IsValid())
// 	{
// 		PlayerController->NotifyWeaponHitTarget(OtherActor, OtherComp->GetComponentLocation());
// 		
// 	}
// }

void APlayerCharacterBase::PerformForwardMovement(const float value)
{
	
	FVector WorldDirection;

	FRotator Rotation = GetControlRotation();

	WorldDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);

	AddMovementInput(WorldDirection * value);
	
}

void APlayerCharacterBase::PerformRightMovement(const float value)
{
	
	FVector WorldDirection;

	FRotator Rotation = GetControlRotation();

	WorldDirection = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);

	AddMovementInput(WorldDirection * value);
	
}

void APlayerCharacterBase::SetHasTargetLocked(bool locked)
{
	SpringArmComponent->bUsePawnControlRotation = !locked;
	GetCharacterMovement()->bOrientRotationToMovement = !locked;	
}

void APlayerCharacterBase::SetSpringArmRotationX(const float value)
{
	FRotator rotation = SpringArmComponent->GetRelativeTransform().GetRotation().Rotator();

	rotation.Yaw += value;

	SpringArmComponent->SetRelativeRotation(rotation);	
}

void APlayerCharacterBase::StartActionJump()
{
	Super::StartActionJump();
}

void APlayerCharacterBase::StopActionJump()
{
	Super::StopActionJump();
}


void APlayerCharacterBase::ToggleCombatMode()
{	
	if (!bIsTogglingCombatMode)
	{
		Super::ToggleCombatMode();

		bIsTogglingCombatMode = true;
	}
}

void APlayerCharacterBase::OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State)
{
	Super::OnAnimationNotify(NotificationName, Type, State);
}

bool APlayerCharacterBase::ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& OutResult)
{
	CombatComp->ComputeCombatResult(InDamageData, OutResult);

	DamageResultList.Add(OutResult);
	SpawnFloatingText();

	return OutResult.HitSuccess;
}

void APlayerCharacterBase::UpdateHealthbar()
{
	if (PlayerController.IsValid())
	{
		PlayerController->UpdatePlayerUI();
	}
}

void APlayerCharacterBase::OnNotifyTransitionCompleted()
{
	if (PlayerController.IsValid())
	{
		bIsTogglingCombatMode = false;
		
		PlayerController->OnNotifyToggleCombatCompleted();
	}
}


void APlayerCharacterBase::AddRewardExp(const uint32 reward)
{
	if (PlayerController.IsValid())
	{
		if (CombatComp->AddRewardExp(reward))
		{
			EquipGear();


			//play level up effects...
		}
	}
}

void APlayerCharacterBase::OnNotifyWeaponHitTarget(AActor* HitActor, const FVector& HitLocation, bool IsLeftWeapon)
{
	FCombatDamageResult Result;
	const UAttackAbility* SelectedAttackAbility = PlayerController->GetSelectedAbility();
	const UAttackAbility* SelectedRageAbility = Cast<URageAttackAbility>(SelectedAttackAbility);
	ABattleCharacterBase* CurrentTarget = PlayerController->GetCurrentTarget();
	uint32 bUsingComboAbility = PlayerController->GetIsUsingCombo();

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

		PlayerController->UpdateEnemyHealthPanel();
		const FString CharName = CombatComp->GetCharacterCombatData()->GetCharacterName().ToString();
		UE_LOG(LogTemp, Warning, TEXT("%s Attacks %s for %f damage!"), *CharName, *HitActor->GetName(), Result.DamageReceived);
	}
}

void APlayerCharacterBase::EnableWeaponCollider(ECollisionEnabled::Type Enable)
{
	if (IsUsingAreaAttack && Enable == ECollisionEnabled::QueryAndPhysics)
	{
		TArray<FHitResult> TargetsHit;
		const FVector StartLocation = GetActorLocation();
		const FVector EndLocation = StartLocation + (GetActorForwardVector() * FVector(50.0f, 0.0f, 0.0f));

		FCollisionShape Sphere = Sphere.MakeSphere(300.0f);

		GetWorld()->SweepMultiByProfile(TargetsHit, StartLocation, EndLocation,
			FQuat::Identity, FName("PlayerWeaponProfile"), Sphere);

		if (TargetsHit.Num() > 0)
		{
			for (int index = 0; index < TargetsHit.Num(); index++)
			{
				ABattleCharacterBase* Target = Cast<ABattleCharacterBase>(TargetsHit[index].Actor);

				if (IsValid(Target))
				{
					FCombatDamageResult DamageResult;
					const UAttackAbility* SelectedAbility = PlayerController->GetSelectedAbility();

					TWeakObjectPtr<UWeaponObject> RightWeapon = EquipmentManager->GetEquippedRightWeapon()->GetWeaponObject();
					TWeakObjectPtr<UWeaponObject> LeftWeapon = EquipmentManager->GetEquippedLeftWeapon()->GetWeaponObject();					bool IsCombo = PlayerController->GetIsUsingCombo();

					if (RightWeapon.IsValid())
					{
						CombatComp->ComputeWeaponDamage(Target, RightWeapon.Get(), SelectedAbility,
							Target->GetActorLocation(), &DamageResult, IsCombo);
					}

					if (LeftWeapon.IsValid())
					{
						CombatComp->ComputeWeaponDamage(Target, LeftWeapon.Get(), SelectedAbility,
							Target->GetActorLocation(), &DamageResult, IsCombo);
					}
				}
			}
		}
	}
	else
	{
		EquipmentManager->SetWeaponCollisionEnabled(Enable);
	}

}

bool APlayerCharacterBase::UnEquipItem(const FInventoryItemInfo* UnequipInfo)
{
	if (IsValid(EquipmentManager))
	{
		if (UnequipInfo->Type == ItemTypes::ITEM_EQUIPABLE_ARMOR)
		{
			UCombatData* CombatData = CombatComp->GetCharacterCombatData();
			return EquipmentManager->UnEquipArmor(CombatData, UnequipInfo->ArmorSlotType);
		}
		else if (UnequipInfo->Type == ItemTypes::ITEM_EQUIPABLE_WEAPON)
		{
			UCombatData* CombatData = CombatComp->GetCharacterCombatData();
			return EquipmentManager->UnEquipWeapon(CombatData, UnequipInfo->bIsLeftHandWeapon);
		}
	}

	return false;
}

bool APlayerCharacterBase::EquipItem(const FInventoryItemInfo* EquipInfo)
{
	if (IsValid(EquipmentManager))
	{
		if (EquipInfo->Type == ItemTypes::ITEM_EQUIPABLE_ARMOR)
		{
			UCombatData* CombatData = CombatComp->GetCharacterCombatData();

			FEquipmentData Armor;

			Armor.ArmorClass = EquipInfo->ItemClass;
			Armor.ArmorMeshComp = nullptr;
			Armor.ArmorSlot = EquipInfo->ArmorSlotType;
			Armor.RootMeshComp = GetMesh();
			Armor.Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
			Armor.WeaponClass = nullptr;
			Armor.WeaponMeshComp = nullptr;

			return EquipmentManager->EquipArmorSlot(&Armor, CombatData);
		}
		else if (EquipInfo->Type == ItemTypes::ITEM_EQUIPABLE_WEAPON)
		{
			UCombatData* CombatData = CombatComp->GetCharacterCombatData();
			FEquipmentData WeaponData;
			WeaponData.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
			WeaponData.WeaponMeshComp = nullptr;
			WeaponData.BattleCharacter = this;
			WeaponData.bCreateMeshComponent = false;
			WeaponData.WeaponClass = EquipInfo->ItemClass;
			WeaponData.RootMeshComp = GetMesh();
			//equip this weapon on equipment comp

			if (EquipInfo->bIsLeftHandWeapon)
			{
				EquipmentManager->EquipLeftHand(&WeaponData, CombatData);
			}
			else
			{
				EquipmentManager->EquipRightHand(&WeaponData, CombatData);
			}

			return true;
		}
	}

	return false;
}

void APlayerCharacterBase::EquipGear()
{
	USkeletalMeshComponent* RootMesh = GetMesh();
	UCombatData* CombatData = CombatComp->GetCharacterCombatData();

	if (IsValid(CombatData))
	{
		if (IsValid(RightWeaponClass))
		{

			FEquipmentData RightWeapon;
			RightWeapon.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
			RightWeapon.WeaponMeshComp = nullptr;
			RightWeapon.BattleCharacter = this;
			RightWeapon.bCreateMeshComponent = false;
			RightWeapon.WeaponClass = RightWeaponClass;
			RightWeapon.RootMeshComp = RootMesh;
			//equip this weapon on equipment comp

			EquipmentManager->EquipRightHand(&RightWeapon, CombatData);

			if (CombatData->GetIsDualWeilding() && IsValid(LeftWeaponClass))
			{
				FEquipmentData LeftWeapon;
				LeftWeapon.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;
				LeftWeapon.WeaponMeshComp = nullptr;
				LeftWeapon.BattleCharacter = this;
				LeftWeapon.bCreateMeshComponent = false;
				LeftWeapon.WeaponClass = LeftWeaponClass;
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

				Armor.ArmorClass = Pairs.Value;
				Armor.ArmorMeshComp = nullptr;
				Armor.ArmorSlot = Pairs.Key;
				Armor.RootMeshComp = RootMesh;
				Armor.Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
				Armor.WeaponClass = nullptr;
				Armor.WeaponMeshComp = nullptr;

				EquipmentManager->EquipArmorSlot(&Armor, CombatData);
			}
		}
	}
}
