// Fill out your copyright notice in the Description page of Project Settings.

#include "JadeVampiressAnimation.h"
#include "JadeVampiress.h"
#include "Characters/AnimNotifies/CharacterAnimNotify.h"

void UJadeVampiressAnimation::SetKnockbackDisabled(bool disabled)
{
	IsKnockedBackOrDisabled = disabled;

	if (IsAttacking && disabled) {
		Montage_Stop(0.1f, UpperBody_AttackMontage);
		Montage_Stop(0.1f, FullBody_AttackMontage);
		Montage_Stop(0.1f, SpecialAttackMontage);
		IsAttacking = false;
	}
}

void UJadeVampiressAnimation::SetIsAlive(bool alive)
{
	IsAlive = alive;

    if (!IsAlive) {
        InCombat = false;
        IsMoving = false;
      
    }

}

void UJadeVampiressAnimation::SetEnableCombatMode(bool enable)
{
    if (!IsJumping && InCombat != enable && !IsInTransition) {
        IsInTransition = true;
        if (enable) {
           
            
        }
        else {
            
        }
    }
    InCombat = enable;
}

void UJadeVampiressAnimation::PlayAttackSection(const FName & SectionName)
{
	if (!IsJumping && !IsKnockedBackOrDisabled && IsAlive && !IsInTransition && InCombat) {
        IsAttacking = true;
		if (IsMoving) {
			Montage_Play(UpperBody_AttackMontage);
		}
		else {
			Montage_Play(FullBody_AttackMontage);
		}
		Montage_JumpToSection(SectionName);
	}
}

void UJadeVampiressAnimation::PlayDamageSection(const FName & SectionName)
{
    if (!IsJumping && !IsAttacking && IsAlive && !IsInTransition) {
        Montage_Play(DamageMontage);
        Montage_JumpToSection(SectionName);
        IsTakingDamage = true;
    }
}

void UJadeVampiressAnimation::PlayTakeDamage()
{
    if (!IsJumping && !IsAttacking && IsAlive && !IsInTransition) {
        if (InCombat) {
            Montage_Play(DamageMontage_UpperBody);
        }
        else {
            Montage_Play(DamageMontage);
        }
        Montage_JumpToSection(FName("TakeDamage"));
        IsTakingDamage = true;
    }
}

void UJadeVampiressAnimation::PlaySpecialSection(const FName & SectionName)
{
	if (!IsJumping && !IsKnockedBackOrDisabled && IsAlive && !IsInTransition && InCombat) {
		Montage_Play(SpecialAttackMontage);
		Montage_JumpToSection(SectionName);
		IsAttacking = true;
	}
}

void UJadeVampiressAnimation::Notify_AttackComplete()
{
    if (IsAlive) {
        AJadeVampiress* JadeCharacter = Cast<AJadeVampiress>(TryGetPawnOwner());
        if (IsValid(JadeCharacter)) {
            JadeCharacter->OnNotifyAttackComplete();
            IsAttacking = false;
        }
    }
}

void UJadeVampiressAnimation::Notify_EnableWeaponCollider()
{
    if (IsAlive) {
        AJadeVampiress* JadeCharacter = Cast<AJadeVampiress>(TryGetPawnOwner());
        if (IsValid(JadeCharacter)) {
            JadeCharacter->EnableWeaponCollider(ECollisionEnabled::QueryAndPhysics);
        }
    }
}

void UJadeVampiressAnimation::Notify_IntroCompleted()
{
    IntroComplete = true;
    PlayerCharacter = Cast<AJadeVampiress>(TryGetPawnOwner());
    if (PlayerCharacter.IsValid()) {
        PlayerCharacter->OnNotifyIntroAnimationCompleted();
    }
}

void UJadeVampiressAnimation::Notify_JumpCompleted()
{
    IsJumping = false;
	IsLanding = false;

	if (PlayerCharacter.IsValid()) {
		PlayerCharacter->OnNotifyJumpCompleted();
	}
}

void UJadeVampiressAnimation::Notify_TransitionCompleted()
{
    IsInTransition = false;
    if (PlayerCharacter.IsValid()) {
        PlayerCharacter->OnNotifyTransitionCompleted();
    }
}

void UJadeVampiressAnimation::PlayStrafeAnimation(float value)
{
    if (HasTargetLock) {
        StrafeValue = value;
        if (value != 0.0f) {
            IsStrafing = true;
            //UE_LOG(LogTemp, Warning, TEXT("Strafe Speed: %f"), value);
        }
        else {
            IsStrafing = false;
        }
    }
}

void UJadeVampiressAnimation::PlayForwardAnimation(float value)
{
    if (HasTargetLock) {
        ForwardValue = value;

        if (value != 0.0f) {
            IsMoving = true;
            //UE_LOG(LogTemp, Warning, TEXT("Forward Speed: %f"), value);
        }
        else {
            IsMoving = false;
        }
    }
}

void UJadeVampiressAnimation::ResetAnimationStates()
{
	IsMoving = false;
	IsTakingDamage = false;
	IsKnockedBackOrDisabled = false;
	Speed = 0.0f;
	MovementDirection = 0.0f;
	IsInTransition = true;
	HasTargetLock = false;
	InCombat = false;
	IntroComplete = false;
	IsJumping = false;
	IsAttacking = false;
}

void UJadeVampiressAnimation::PerformJumpAnimation()
{
	if (!IsJumping && !IsInTransition)
	{
		IsJumping = true;
		if (IsValid(JumpMontage) && !InCombat)
		{
			Montage_Play(JumpMontage);
			Montage_JumpToSection(FName("JumpStart"), JumpMontage);
		}
		else if (IsValid(CombatJumpMontage) && InCombat)
		{
			Montage_Play(CombatJumpMontage);
			Montage_JumpToSection(FName("JumpStart"), CombatJumpMontage);
		}
	}
	
}

void UJadeVampiressAnimation::PerformFinishJumpAnimation()
{
	if (IsJumping)
	{
		IsLanding = true;

		if (IsValid(JumpMontage) && !InCombat)
		{
			if (FMath::IsNearlyZero(Speed))
			{
				Montage_Play(JumpMontage);
				Montage_JumpToSection(FName("JumpLand"), JumpMontage);
			}
			else
			{
				Montage_Stop(0.1f, JumpMontage);
				Notify_JumpCompleted();
			}
		}
		else if (IsValid(CombatJumpMontage) && InCombat)
		{
			if (FMath::IsNearlyZero(Speed))
			{
				Montage_Play(CombatJumpMontage);
				Montage_JumpToSection(FName("JumpLand"), CombatJumpMontage);
			}
			else
			{
				Montage_Stop(0.1f, CombatJumpMontage);
				Notify_JumpCompleted();
			}
		}
	}
}

void UJadeVampiressAnimation::OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State)
{
	switch (Type)
	{
	case NotifyTypes::NOTIFY_ANIM_GENERAIC:
		break;
	case NotifyTypes::NOTIFY_ATTACK:
		break;
	case NotifyTypes::NOTIFY_RAGEATTACK:
		break;
	case NotifyTypes::NOTIFY_COLISION:
	{
		switch (State)
		{
		case NotifyTypeStates::NOTIFY_STATE_ENABLED:
			Notify_EnableWeaponCollider();
			break;
		case NotifyTypeStates::NOTIFY_STATE_DISABLED:
			break;
		default:
			break;
		}
		break;
	}
	case NotifyTypes::NOTIFY_TRANSITION:
		break;
	case NotifyTypes::NOTIFY_INTROANIM:
		break;
	case NotifyTypes::NOTIFY_JUMP:
		break;
	default:
		break;
	}
}

void UJadeVampiressAnimation::NativeInitializeAnimation()
{
	Super::NativeInitializeAnimation();
	
	ResetAnimationStates();
   
}

void UJadeVampiressAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);
    if (PlayerCharacter.IsValid()) {
        FVector velocity = PlayerCharacter->GetVelocity();
        FRotator Rotation = PlayerCharacter->GetControlRotation();
        FVector Direction;

		velocity.Z = 0.0f;

        velocity.ToDirectionAndLength(Direction, Speed);

		if (IsJumping && IsLanding && Speed > 0.0f)
		{
			Notify_JumpCompleted();
		}

        if (!IsJumping && !HasTargetLock && Speed > 0.0f) {
            ForwardValue = Speed;
            IsMoving = (ForwardValue != 0.0f) ? true : false;
            
            IsStrafing = false;
            StrafeValue = 0.0f;
            //UE_LOG(LogTemp, Warning, TEXT("Velocity = %f %f %f  Speed: %f"), velocity.X, velocity.Y, velocity.Z, Speed);
        }
        else if (!IsJumping && !HasTargetLock && Speed == 0.0f && ForwardValue != 0.0f) {
            ForwardValue = 0.0f;
            IsMoving = false;
        }
        else if (Speed == 0.0f)
        {
            ForwardValue = 0.0f;
            IsMoving = false;
            IsStrafing = false;
            StrafeValue = 0.0f;
        }
        
    }
}

void UJadeVampiressAnimation::NativeBeginPlay()
{
	Super::NativeBeginPlay();
    PlayerCharacter = Cast<APlayerCharacterBase>(TryGetPawnOwner());
    IsAlive = true;
    
   
}
