// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyAIControllerBase.h"
#include "Runtime/Engine/Classes/AI/NavigationSystembase.h"
#include "NavigationSystem.h"
#include "Runtime/AIModule/Classes/BrainComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "Runtime/AIModule/Classes/Perception/AIPerceptionComponent.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Sight.h"
#include "Runtime/AIModule/Classes/Perception/AISenseConfig_Damage.h"
#include "Characters/Players/PlayerCharacterBase.h"
#include "Characters/Enemies/EnemyCharacterBase.h"
#include "Characters/Enemies/Components/LootSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Engine.h"

AEnemyAIControllerBase::AEnemyAIControllerBase() : Super()
{
	SetGenericTeamId(FGenericTeamId(1));

	PerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception Component"));

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	DamageConfig = CreateDefaultSubobject<UAISenseConfig_Damage>(TEXT("DamageConfig"));

	PerceptionComponent->OnTargetPerceptionUpdated.AddDynamic(this, &AEnemyAIControllerBase::TargetPerceptionUpdate);

	AbilityComponent = CreateDefaultSubobject<UAbilityManagerComponent>(TEXT("Ability Component"));
	
}

 

void AEnemyAIControllerBase::Kill()
{
	GetBrainComponent()->StopLogic(FString("Dead"));

}

void AEnemyAIControllerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemyAIControllerBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}


void AEnemyAIControllerBase::PreInitializeComponents()
{
	Super::PreInitializeComponents();

}

void AEnemyAIControllerBase::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
   
}

void AEnemyAIControllerBase::Possess(APawn * InPawn)
{
	EnemyCharacter = Cast<AEnemyCharacterBase>(InPawn);

	if (EnemyCharacter) 
	{
		EnemyAnimation = Cast<UEnemyAnimationBasse>(EnemyCharacter->GetMesh()->GetAnimInstance());

		if (EnemyAnimation.IsValid() && TreeClassToRun)
		{

			RunBehaviorTree(TreeClassToRun);


			Super::Possess(InPawn);
		}
	}
}

void AEnemyAIControllerBase::UnPossess()
{
	Super::UnPossess();

}

bool AEnemyAIControllerBase::PerformMoveTo(const FVector & Destination, float MoveRadius)
{
    //UE_LOG(LogTemp, Warning, TEXT("PerformMoveTo()"));
	bool success = false;

	EPathFollowingStatus::Type Status = GetMoveStatus();
	if (Status != EPathFollowingStatus::Idle) {
        //UE_LOG(LogTemp, Warning, TEXT("I was moving so i stopped...."));
		StopMovement();
	}

	EPathFollowingRequestResult::Type Results;

    //UE_LOG(LogTemp, Warning, TEXT("Creating Random Movement route..."));
	Results = MoveToLocation(Destination, MoveRadius, false, true, true);

	switch (Results) {

	case EPathFollowingRequestResult::AlreadyAtGoal:
       // UE_LOG(LogTemp, Warning, TEXT("I am already close to that destination...."));
		AIConfiguration.IsMoving = false;
		AIConfiguration.IsChasing = false;
		success = true;

		EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
		EnemyAnimation->IsMoving = AIConfiguration.IsMoving;
		break;

	case EPathFollowingRequestResult::RequestSuccessful:
       // UE_LOG(LogTemp, Warning, TEXT("Moving now...To Random Destingation: %f %f %f"), Destination.X, Destination.Y, Destination.Z);
		AIConfiguration.IsMoving = true;
		AIConfiguration.IsChasing = false;
		EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
		EnemyAnimation->IsMoving = AIConfiguration.IsMoving;
		success = true;
		break;

	case EPathFollowingRequestResult::Failed:
       // UE_LOG(LogTemp, Warning, TEXT("Nope....Cant Go There..."));
		AIConfiguration.IsMoving = false;
		AIConfiguration.IsChasing = false;
		break;
	}

	
	return success;
}

bool AEnemyAIControllerBase::PerformChaseTarget(bool &AlreadyAtGoal)
{
    //UE_LOG(LogTemp, Warning, TEXT("PerformChaseTarget()"));
	bool success = false;
	AlreadyAtGoal = false;

	EPathFollowingStatus::Type Status = GetMoveStatus();
	if (Status != EPathFollowingStatus::Idle) {
		StopMovement();
        //UE_LOG(LogTemp, Warning, TEXT("I Was Moving, now i stopped!"));
	}

	EPathFollowingRequestResult::Type Results;

    //UE_LOG(LogTemp, Warning, TEXT("Creating Movement To Player!"));
	Results = MoveToActor(SensedTarget.Get(), AIConfiguration.MoveToTargetStopDistance);
	switch (Results) {

	case EPathFollowingRequestResult::AlreadyAtGoal:
       // UE_LOG(LogTemp, Warning, TEXT("Already At Goal!"));
		AIConfiguration.IsMoving = false;
		AIConfiguration.IsChasing = false;
		success = false;

		AlreadyAtGoal = true;

		EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
		EnemyAnimation->IsMoving = AIConfiguration.IsMoving;
		break;

	case EPathFollowingRequestResult::RequestSuccessful:
        //UE_LOG(LogTemp, Warning, TEXT("Moving To Player Now!"));
		AIConfiguration.IsMoving = false;
		AIConfiguration.IsChasing = true;
		EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
		EnemyAnimation->IsMoving = AIConfiguration.IsMoving;
		success = true;
		break;

	case EPathFollowingRequestResult::Failed:
       // UE_LOG(LogTemp, Warning, TEXT("I failed to move towards player!"));
		AIConfiguration.IsMoving = false;
		AIConfiguration.IsChasing = false;
		break;
	}

	
	return success;
}

bool AEnemyAIControllerBase::PerformAttackTarget()
{
	bool success = false;

	if (EnemyCharacter && EnemyCharacter->IsValidLowLevel()) {
		if (ChooseRandomAbility())
		{
			if (SelectedAttackAbility.IsValid())
			{
				success = EnemyCharacter->PerformSingleAttack(SelectedAttackAbility->AnimationSectionName);
			}
			if (SelectedRageAbility.IsValid())
			{
				success = EnemyCharacter->PerformRageAttack(SelectedRageAbility->AnimationSectionName);
			}
		}
	}
	return success;
}

bool AEnemyAIControllerBase::IsInAttackingRange()
{
	bool success = false;
	if (EnemyCharacter && EnemyCharacter->IsValidLowLevel() && SensedTarget.IsValid() && SensedTarget->IsValidLowLevel()) {
		FVector EnemyLocation = EnemyCharacter->GetActorLocation();
		FVector PlayerLocation = SensedTarget->GetActorLocation();
		FVector Distance = PlayerLocation - EnemyLocation;
		FVector Direction;

		float distance = 0.0f;

		Distance.ToDirectionAndLength(Direction, distance);

		if (distance <= AIConfiguration.AttackRangeMax && distance >= AIConfiguration.AttackRangeMin) {
			success = true;
		}
	}

	return success;
}

void AEnemyAIControllerBase::AlertDamageEvent(AActor* ActorInstigator, const float InDamage)
{
	if (IsValid(GetPawn()))
	{
		UAISense_Damage::ReportDamageEvent(this, this, ActorInstigator, InDamage, GetPawn()->GetActorLocation(), FVector());
	}
}

void AEnemyAIControllerBase::ActorsPerceptionUpdated(const TArray<AActor*>& UpdatedActors)
{
	
}

void AEnemyAIControllerBase::TargetPerceptionUpdate(AActor * Actor, FAIStimulus Stimulus)
{
    //UE_LOG(LogTemp, Warning, TEXT("TargetPerceptionUpdate()!"));

	FActorPerceptionBlueprintInfo Info;
	if (PerceptionComponent->GetActorsPerception(Actor, Info)) {
        //UE_LOG(LogTemp, Warning, TEXT("I Sensed Something!"));
		TWeakObjectPtr<APlayerCharacterBase> Player = Cast<APlayerCharacterBase>(Actor);

		if (Stimulus.WasSuccessfullySensed() || Info.LastSensedStimuli[0].WasSuccessfullySensed()) {
			if (Player.IsValid()) {
                //UE_LOG(LogTemp, Warning, TEXT("Player I See YOU!"));
				AIConfiguration.HasTargetSighted = true;
				SensedTarget = Player;
				StopMovement();
				
				if (EnemyCharacter) {
					EnemyCharacter->EnableCombatMode(true);
				}
				
			}
		}
		else {
            //UE_LOG(LogTemp, Warning, TEXT("Lost Sight of Target!"));
			SensedTarget = nullptr;
			AIConfiguration.HasTargetSighted = false;
			AIConfiguration.IsEngaging = false;
			StopMovement();
		}
	}

	if (SensedTarget == nullptr) {
		
		if (EnemyCharacter) {
			EnemyCharacter->EnableCombatMode(false);
		}
	}
}


void AEnemyAIControllerBase::NotifyAttackComplete()
{
	AIConfiguration.IsAttacking = false;
	if (IsValid(EnemyCharacter))
	{
		EnemyCharacter->OnNotifyAttackComplete();
	}
}

void AEnemyAIControllerBase::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult & Result)
{
	Super::OnMoveCompleted(RequestID, Result);

    //UE_LOG(LogTemp, Warning, TEXT("Movement Completed!"));
	AIConfiguration.IsMoving = false;
	AIConfiguration.IsChasing = false;
	EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
	EnemyAnimation->IsMoving = AIConfiguration.IsMoving;


}

APlayerCharacterBase * AEnemyAIControllerBase::GetSensedTarget()
{
	if (SensedTarget.IsValid()) {
		return SensedTarget.Get();
	}
	return nullptr;
}

void AEnemyAIControllerBase::ActivatePerception()
{
    PerceptionComponent->ConfigureSense(*SightConfig);
    PerceptionComponent->SetSenseEnabled(SightConfig->GetSenseImplementation(), true);
    PerceptionComponent->ConfigureSense(*DamageConfig);
    PerceptionComponent->SetSenseEnabled(DamageConfig->GetSenseImplementation(), true);
    AIConfiguration.IsPerceptionActive = true;
    //UE_LOG(LogTemp, Warning, TEXT("Perception Activated!"));
}

void AEnemyAIControllerBase::AbortMoveTo()
{
    //UE_LOG(LogTemp, Warning, TEXT("AbortMoveTo()"));
    AIConfiguration.IsMoving = false;
    AIConfiguration.IsChasing = false;
    EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
    EnemyAnimation->IsMoving = AIConfiguration.IsMoving;

    StopMovement();
}

void AEnemyAIControllerBase::RerouteChasePlayer()
{
    //UE_LOG(LogTemp, Warning, TEXT("ReRouteChasePlayer()"));
    EPathFollowingStatus::Type Status = GetMoveStatus();
    if (Status != EPathFollowingStatus::Idle) {
        //UE_LOG(LogTemp, Warning, TEXT("Stoping Movement!"));
        StopMovement();
    }

	const UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(GetWorld());
    const UNavigationSystem* Navigation = Cast<UNavigationSystem>(GetWorld()->GetNavigationSystem());
    FVector Destination;

    if (IsValid(NavSystem))
    {
        if (NavSystem->K2_GetRandomReachablePointInRadius(this, EnemyCharacter->GetActorLocation(), Destination, 250.0f))
        {
            //UE_LOG(LogTemp, Warning, TEXT("New Nav Point Found: %f %f %f"), Destination.Location.X, Destination.Location.Y, Destination.Location.Z);
            EPathFollowingRequestResult::Type Results;

            Results = MoveToLocation(Destination, 10.0f, false, true, true);

            switch (Results) {

            case EPathFollowingRequestResult::AlreadyAtGoal:
                //lets not change anything, if we bumped something we will come into this call again
                //UE_LOG(LogTemp, Warning, TEXT("Already At Goal!"));
                break;

            case EPathFollowingRequestResult::RequestSuccessful:
                //UE_LOG(LogTemp, Warning, TEXT("Ive Started Moving!"));
                AIConfiguration.IsChasing = true;
                EnemyAnimation->IsChasing = AIConfiguration.IsChasing;
                break;

            case EPathFollowingRequestResult::Failed:
                //UE_LOG(LogTemp, Warning, TEXT("Movement Failed!"));
                //still remain chase even tho we stopped because we will probably try to move again on fail
                break;
            }
        }
    }
}

void AEnemyAIControllerBase::LookAtSensedTarget()
{
	if (AIConfiguration.HasTargetSighted && SensedTarget.IsValid())
	{
		FVector TargetPosition = SensedTarget->GetActorLocation();

		FVector Direction = (TargetPosition - EnemyCharacter->GetActorLocation());

		FRotator LookAtRot = FRotationMatrix::MakeFromX(Direction).Rotator();

		SetControlRotation(LookAtRot);
		EnemyCharacter->SetActorRotation(LookAtRot);
	}
}

void AEnemyAIControllerBase::UpdateOriginAI()
{
	if (IsValid(EnemyCharacter))
	{
		AIConfiguration.HomePosition = EnemyCharacter->GetActorLocation();
	}
}


bool AEnemyAIControllerBase::ChooseRandomAbility()
{

	const TMap<FName, FAbilityInfo>* Abilities = AbilityComponent->GetEquippedAbilities();

	if (Abilities && Abilities->Num() > 0)
	{
		TArray<FName> Keys;
		
		Abilities->GenerateKeyArray(Keys);

		const int random = (FMath::RandRange(0, Keys.Num() - 1));

		const FAbilityInfo* Ability = Abilities->Find(Keys[random]);
		
		if (Ability && Ability->bIsEnabled)
		{
			const UCombatData* CombatData = EnemyCharacter->GetCharacterCombatData();
			if (AbilityComponent->UseAbility(Ability->KeyName, &CombatData))
			{
				const UAttackAbility* Attack = Cast<UAttackAbility>(AbilityComponent->GetGenericAbility(Ability->KeyName));
				const URageAttackAbility* Rage = Cast<URageAttackAbility>(Attack);

				if (IsValid(Attack))
				{
					SelectedAttackAbility = (UAttackAbility*)Attack;
					return true;
				}
				else if (IsValid(Rage))
				{
					SelectedRageAbility = (URageAttackAbility*)Rage;
					return true;
				}
			}
		}
		
	}
	return false;
}

void AEnemyAIControllerBase::SetCurrentLevel(const uint8 level)
{
	EnemyCharacter->SetEnemyLevel(level);
}


const UAttackAbility* AEnemyAIControllerBase::GetSelectedAbility()
{
	if (SelectedAttackAbility.IsValid())
	{
		return SelectedAttackAbility.Get();
	}
	else if (SelectedRageAbility.IsValid())
	{
		return SelectedRageAbility.Get();
	}

	return nullptr;
}
