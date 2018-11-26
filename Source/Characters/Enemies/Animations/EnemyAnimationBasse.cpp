// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAnimationBasse.h"

void UEnemyAnimationBasse::PlayAnimationSection(FName SectionName)
{
	if (SectionName.IsValid() && IsValid(AttackAnimations)) {
		Montage_Play(AttackAnimations);
		Montage_JumpToSection(SectionName);
		IsAttacking = true;
	}
}

void UEnemyAnimationBasse::PlayTakeHitAnimation()
{
	if (IsValid(AttackAnimations) && !IsAttacking) {
		Montage_Play(AttackAnimations);
		if (InBattle) {
			Montage_JumpToSection(FName("TakeHitCombat"));
		}
		else {
			Montage_JumpToSection(FName("TakeHitNormal"));
		}
		TakeHit = true;
	}
}

void UEnemyAnimationBasse::PlayDeathAnimation()
{
	TakeHit = false;
	IsAttacking = false;
	IsMoving = false;
	IsChasing = false;
	IsAlive = false;
}
