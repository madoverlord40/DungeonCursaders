// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_IdleRoam.h"
#include "../EnemyAIControllerBase.h"
#include "Runtime/Engine/Classes/AI/NavigationSystembase.h"
#include "NavigationSystem.h"

UAITask_IdleRoam::UAITask_IdleRoam()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = false;
}

EBTNodeResult::Type UAITask_IdleRoam::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishFailedValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr)
        {
            if (AIConfig->HasTargetSighted == false)
            {
				//getting errors to change to this but its not getting correct nav points
				//UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(AIController->GetWorld());
				
				UNavigationSystemBase* NavBase = AIController->GetWorld()->GetNavigationSystem();
				UNavigationSystemV1* NavSystem = Cast<UNavigationSystemV1>(NavBase);
                
				if (IsValid(NavSystem))
                {
					FVector MoveToLocation = FVector::ZeroVector;
					//MoveToLocation = NavSystem->GetRandomReachablePointInRadius(AIController.Get(), AIConfig->HomePosition, AIConfig->RoamingRadius);
					//NavSystem->K2_GetRandomReachablePointInRadius(this, AIConfig->HomePosition, MoveToLocation, AIConfig->RoamingRadius);
					UNavigationSystemV1::K2_GetRandomPointInNavigableRadius(AIController.Get(), AIConfig->HomePosition, MoveToLocation, AIConfig->RoamingRadius);

                    if (AIController->PerformMoveTo(MoveToLocation, AIConfig->RoamMoveAcceptRadius))
                    {
                        bCanUseTick = true;
                        Result = EBTNodeResult::InProgress;
                    }
                }
            }
        }
    }
    return Result;
}

EBTNodeResult::Type UAITask_IdleRoam::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_IdleRoam::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    if (bCanUseTick) {
        TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

        if (AIController.IsValid())
        {
            const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
            if (AIConfig != nullptr)
            {
                if (AIConfig->IsMoving == false || AIConfig->HasTargetSighted)
                {
                    FinishLatentTask(OwnerComp, FinishSuccessValue);
                    bCanUseTick = false;
                }
            }
            else
            {
                FinishLatentTask(OwnerComp, FinishFailedValue);
                bCanUseTick = false;
            }
        }
    }
}

void UAITask_IdleRoam::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
}
