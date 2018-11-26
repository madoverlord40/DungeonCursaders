// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "EnemyAnimationBasse.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEnemyAnimationBasse : public UAnimInstance
{
	GENERATED_BODY()
	
public:

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsMoving = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsAttacking = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsChasing = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool TakeHit = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool IsAlive = true;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	bool InBattle = false;

	UPROPERTY(EditAnyWhere, BlueprintReadWrite)
	UAnimMontage* AttackAnimations;


public:

	UFUNCTION(BlueprintCallable)
	void PlayAnimationSection(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void PlayTakeHitAnimation();


	UFUNCTION(BlueprintCallable)
	void PlayDeathAnimation();

};
