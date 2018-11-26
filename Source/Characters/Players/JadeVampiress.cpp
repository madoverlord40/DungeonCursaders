// Fill out your copyright notice in the Description page of Project Settings.

#include "JadeVampiress.h"
#include "Characters/AnimNotifies/CharacterAnimNotify.h"
#include "Runtime/Engine/Classes/GameFramework/SpringArmComponent.h"
#include "Runtime/Engine/Classes/Camera/CameraComponent.h"
#include "Runtime/Engine/Classes/Components/InputComponent.h"
#include "Runtime/Engine/Classes/GameFramework/PlayerController.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "JadeVampiressAnimation.h"
#include "../Enemies/EnemyCharacterBase.h"
#include "Controllers/PlayerControllerBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"


AJadeVampiress::AJadeVampiress() : APlayerCharacterBase()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	GenericTeamID = FGenericTeamId(2);

}

void AJadeVampiress::BeginPlay()
{
	Super::BeginPlay();

}

void AJadeVampiress::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AJadeVampiress::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	
}

void AJadeVampiress::PostInitializeComponents()
{
	Super::PostInitializeComponents();

    TWeakObjectPtr<UAnimInstance> AnimationInstance = GetMesh()->GetAnimInstance();

    if (AnimationInstance.IsValid()) {
        Animation = Cast<UJadeVampiressAnimation>(AnimationInstance);

    }
	
	
}

void AJadeVampiress::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);
	PlayerController = Cast<APlayerControllerBase>(NewController);
}

void AJadeVampiress::UnPossessed()
{
	Super::UnPossessed();

}

void AJadeVampiress::Falling()
{
	Super::Falling();

	if (!IsJumping)
	{

	}
}

void AJadeVampiress::NotifyJumpApex()
{
	if (IsJumping && IsValid(Animation))
	{
		
	}
}

void AJadeVampiress::StartActionJump()
{
    if (IsValid(Animation) && !IsJumping)
	{
		Super::StartActionJump();
		Animation->PerformJumpAnimation();
	}
}

void AJadeVampiress::StopActionJump()
{
    Super::StopActionJump();

	
}

void AJadeVampiress::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);

	if (IsValid(Animation) && IsJumping)
	{
		Animation->PerformFinishJumpAnimation();
	}
}

void AJadeVampiress::MoveBlockedBy(const FHitResult & Impact)
{
	Super::MoveBlockedBy(Impact);

}


bool AJadeVampiress::PerformAreaAttack(const FName& AnimSectionName, bool IsRageAttack)
{
	if (Super::PerformAreaAttack(AnimSectionName, IsRageAttack))
	{
		Animation->PlayAttackSection(AnimSectionName);
		IsAttacking = true;
		IsUsingAreaAttack = true;
	}

	return IsAttacking;

}

bool AJadeVampiress::PerformSingleAttack(const FName& AnimSectionName)
{
	if (Super::PerformSingleAttack(AnimSectionName))
	{
		Animation->PlayAttackSection(AnimSectionName);
		IsAttacking = true;
	}

	return IsAttacking;
}


bool AJadeVampiress::PerformRageAttack(const FName& AnimSectionName)
{
	if (Super::PerformRageAttack(AnimSectionName))
	{
		Animation->PlaySpecialSection(AnimSectionName);
		IsAttacking = true;
	}

	return IsAttacking;
}

void AJadeVampiress::PerformForwardMovement(const float value)
{
    Super::PerformForwardMovement(value);

    if (IsValid(Animation)) {
        Animation->PlayForwardAnimation(value);
    }
}

void AJadeVampiress::PerformRightMovement(const float value)
{
    Super::PerformRightMovement(value);
    if (IsValid(Animation)) {
        Animation->PlayStrafeAnimation(value);
    }
}

void AJadeVampiress::SetHasTargetLocked(bool locked)
{
    Super::SetHasTargetLocked(locked);

    if (IsValid(Animation)) {
        Animation->HasTargetLock = locked;
    }
}

void AJadeVampiress::OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State)
{
	Super::OnAnimationNotify(NotificationName, Type, State);
	if (IsValid(Animation)) {
		Animation->OnAnimationNotify(NotificationName, Type, State);
	}
}

void AJadeVampiress::OnNotifyAttackComplete()
{
	Super::OnNotifyAttackComplete();

	if (PlayerController.IsValid())
	{
		PlayerController->NotifyAttackComplete();
	}

	
}

void AJadeVampiress::ToggleCombatMode()
{
    if (IsValid(Animation)) {
        
        Super::ToggleCombatMode();
        Animation->SetEnableCombatMode(IsInCombat);
    }
}

void AJadeVampiress::SetGenericTeamId(const FGenericTeamId & NewTeamID)
{
	GenericTeamID = NewTeamID;
}


