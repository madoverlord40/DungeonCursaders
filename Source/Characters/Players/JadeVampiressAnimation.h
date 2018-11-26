// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "Runtime/Engine/Classes/Animation/BlendSpace1D.h"
#include "JadeVampiressAnimation.generated.h"


enum class NotifyTypes : uint8;
enum class NotifyTypeStates : uint8;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UJadeVampiressAnimation : public UAnimInstance
{
	GENERATED_BODY()
	
	
protected:
 
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* CombatJumpMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* UpperBody_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* FullBody_AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* DamageMontage;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UAnimMontage* DamageMontage_UpperBody;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UAnimMontage* SpecialAttackMontage;

 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAttacking;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsTakingDamage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsAlive;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsMoving;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool IsStrafing;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool IsJumping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsLanding;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	bool IsKnockedBackOrDisabled;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool InCombat;

    UPROPERTY(EditAnywhere, BlueprintReadWrite)
    bool IntroComplete = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool IsInTransition = false;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float StrafeValue = 0.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float ForwardValue = 0.0f;

    TWeakObjectPtr<class APlayerCharacterBase> PlayerCharacter;

public:
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    bool HasTargetLock;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float MovementDirection;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float Speed;

	UFUNCTION(BlueprintCallable)
	void SetKnockbackDisabled(bool disabled);

	UFUNCTION(BlueprintCallable)
	void SetIsAlive(bool alive);

    void SetEnableCombatMode(bool enable);

	UFUNCTION(BlueprintCallable)
	void PlayAttackSection(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void PlayDamageSection(const FName& SectionName);

    void PlayTakeDamage();

	UFUNCTION(BlueprintCallable)
	void PlaySpecialSection(const FName& SectionName);

	UFUNCTION(BlueprintCallable)
	void Notify_AttackComplete();

	UFUNCTION(BlueprintCallable)
	void Notify_EnableWeaponCollider();

    UFUNCTION(BlueprintCallable)
    void Notify_IntroCompleted();

    UFUNCTION(BlueprintCallable)
    void Notify_JumpCompleted();

    UFUNCTION(BlueprintCallable)
    void Notify_TransitionCompleted();

    void SetTargetLock(bool enable) { HasTargetLock = enable; }

    void PlayStrafeAnimation(float value);
    void PlayForwardAnimation(float value);

	void ResetAnimationStates();

	void PerformJumpAnimation();

	void PerformFinishJumpAnimation();

	void OnAnimationNotify(const FName& NotificationName, const NotifyTypes Type, const NotifyTypeStates State);

	// the below functions are the native overrides for each phase
	// Native initialization override point
	virtual void NativeInitializeAnimation() override;
	// Native update override point. It is usually a good idea to simply gather data in this step and 
	// for the bulk of the work to be done in NativeUpdateAnimation.
	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	// Executed when begin play is called on the owning component
	virtual void NativeBeginPlay() override;
};
