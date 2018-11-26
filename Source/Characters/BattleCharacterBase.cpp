// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleCharacterBase.h"
#include "AnimNotifies/CharacterAnimNotify.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "FloatingDamageTextActor.h"
#include "DrawDebugHelpers.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "Players/Controllers/PlayerControllerBase.h"
#include "Characters/Players/Components/EquipmentComponent.h"
#include "Characters/Players/Components/CombatComponent.h"


ABattleCharacterBase::ABattleCharacterBase() : ACharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	CombatComp = CreateDefaultSubobject<UCombatComponent>(TEXT("CombatComponent"));

	EquipmentManager = CreateDefaultSubobject<UEquipmentComponent>(TEXT("EquipmentComp"));

	BodyCollision = CreateDefaultSubobject<UCapsuleComponent>(TEXT("BodyCollision"));
	BodyCollision->SetupAttachment(GetMesh(), BodyCollisionSocketName);
	
}



// Called when the game starts or when spawned
void ABattleCharacterBase::BeginPlay()
{
	ACharacter::BeginPlay();
    IsAlive = true;
    
}

// Called every frame
void ABattleCharacterBase::Tick(float DeltaTime)
{
	ACharacter::Tick(DeltaTime);
	
}

// Called to bind functionality to input
void ABattleCharacterBase::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	ACharacter::SetupPlayerInputComponent(PlayerInputComponent);

}

void ABattleCharacterBase::PostInitializeComponents()
{
	ACharacter::PostInitializeComponents();

	GetMesh()->SetCanEverAffectNavigation(false);

	BodyCollision->AttachToComponent(GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, BodyCollisionSocketName);
	BodyCollision->SetCapsuleHalfHeight(60.0f);
	BodyCollision->SetCapsuleRadius(45.0f);
	BodyCollision->SetCollisionProfileName(BodyCollisionProfileName);
	BodyCollision->SetGenerateOverlapEvents(true);
	BodyCollision->SetUseCCD(true);
	BodyCollision->SetCanEverAffectNavigation(false);
	BodyCollision->bMultiBodyOverlap = true;

}

void ABattleCharacterBase::NotifyJumpApex()
{
	ACharacter::NotifyJumpApex();

}

void ABattleCharacterBase::Landed(const FHitResult & Hit)
{
	ACharacter::Landed(Hit);

	
}

void ABattleCharacterBase::MoveBlockedBy(const FHitResult & Impact)
{
	ACharacter::MoveBlockedBy(Impact);

}

void ABattleCharacterBase::OnNotifyJumpCompleted()
{
	IsJumping = false;
}

void ABattleCharacterBase::OnNotifyAttackComplete()
{
	IsAttacking = false;
	IsUsingRageAttack = false;
	IsUsingAreaAttack = false;
}

bool ABattleCharacterBase::PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack)
{
	bool success = false;

	if (!IsAttacking && !IsUsingAreaAttack && !IsUsingRageAttack && !IsJumping)
	{
		IsUsingAreaAttack = true;
		success = true;
	}
	
	return success;
}


bool ABattleCharacterBase::PerformRageAttack(const FName& AnimSectionName)
{
	bool success = false;

	if (!IsAttacking && !IsJumping && !IsUsingRageAttack && IsInCombat)
	{
		success = true;
	}

	return success;
}


bool ABattleCharacterBase::PerformSingleAttack(const FName& AnimSectionName)
{
	bool success = false;
	if (IsInCombat && !IsAttacking && !IsUsingAreaAttack &&  !IsUsingRageAttack && !IsJumping)
	{
		success = true;
	}

	return success;
}

void ABattleCharacterBase::StartActionJump()
{
	if (!IsAttacking && !IsUsingAreaAttack && !IsUsingRageAttack)
	{
		Jump();
		IsJumping = true;
	}
}

void ABattleCharacterBase::StopActionJump()
{
	StopJumping();
	
}

void ABattleCharacterBase::ToggleCombatMode()
{
	IsInCombat = !IsInCombat;
}

bool ABattleCharacterBase::ComputeHitAndDamage(const FCombatDamageData* InDamageData, FCombatDamageResult& DamageResult)
{
	return false;
}

void ABattleCharacterBase::Kill()
{
    IsAlive = false;
	BodyCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
}

const UCombatData* ABattleCharacterBase::GetCharacterCombatData() const
{
	return CombatComp->GetCharacterCombatData();
}

const UCombatComponent* ABattleCharacterBase::GetCombatControler() const
{
	return CombatComp;
}

void ABattleCharacterBase::EnableWeaponCollider(ECollisionEnabled::Type Enable)
{
	
}

void ABattleCharacterBase::OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State)
{

}

void ABattleCharacterBase::SpawnFloatingText()
{
	if (DamageResultCounter < DamageResultList.Num())
	{
		AFloatingDamageTextActor* DamageTextActor = GetWorld()->SpawnActor<AFloatingDamageTextActor>(AFloatingDamageTextActor::StaticClass());
		DamageTextActor->AttachToActor(this, FAttachmentTransformRules::KeepRelativeTransform);
		DamageTextActor->SetText(DamageResultList[DamageResultCounter].ResultText.ToString());

		DamageResultCounter++;
	}
	else {
		DamageResultCounter = 0;
		DamageResultList.Empty();
	}
}

