// Fill out your copyright notice in the Description page of Project Settings.

#include "UndeadWarrior.h"
#include "Runtime/Engine/Classes/Components/CapsuleComponent.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "../Players/PlayerCharacterBase.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Animations/EnemyAnimationBasse.h"


AUndeadWarrior::AUndeadWarrior()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}


void AUndeadWarrior::BeginPlay()
{
	Super::BeginPlay();

}

void AUndeadWarrior::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AUndeadWarrior::SetupPlayerInputComponent(UInputComponent * PlayerInputComponent)
{
}

void AUndeadWarrior::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	

}

void AUndeadWarrior::PossessedBy(AController * NewController)
{
	Super::PossessedBy(NewController);

}

void AUndeadWarrior::UnPossessed()
{
	Super::UnPossessed();

}

void AUndeadWarrior::Landed(const FHitResult & Hit)
{
	Super::Landed(Hit);

}

void AUndeadWarrior::MoveBlockedBy(const FHitResult & Impact)
{
	Super::MoveBlockedBy(Impact);

}


void AUndeadWarrior::OnNotifyAttackComplete()
{
	Super::OnNotifyAttackComplete();
}
