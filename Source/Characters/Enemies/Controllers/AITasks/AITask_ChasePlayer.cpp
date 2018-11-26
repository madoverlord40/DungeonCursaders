// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_ChasePlayer.h"
#include "../EnemyAIControllerBase.h"
#include "Runtime/Engine/Classes/AI/NavigationSystembase.h"


UAITask_ChasePlayer::UAITask_ChasePlayer()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = false;
}

EBTNodeResult::Type UAITask_ChasePlayer::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishFailedValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());
    bool atgoal = false;

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr)
        {
            if (AIConfig->HasTargetSighted)
            {
                if (AIController->PerformChaseTarget(atgoal))
                {
                    if (atgoal == false)
                    {
                        bCanUseTick = true;
                        Result = EBTNodeResult::InProgress;
                    }
                    else
                    {
                        Result = FinishSuccessValue;
                    }
                }
                else if (atgoal)
                {
                    Result = FinishSuccessValue;
                }
            }
        }
    }
    return Result;
}

EBTNodeResult::Type UAITask_ChasePlayer::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_ChasePlayer::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    if (bCanUseTick) {
        TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

        if (AIController.IsValid())
        {
            const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
            if (AIConfig != nullptr && AIConfig->HasTargetSighted)
            {

                if (AIConfig->IsChasing == false && AIController->IsInAttackingRange())
                {
                    FinishLatentTask(OwnerComp, FinishSuccessValue);
                    bCanUseTick = false;
                }
                else if(AIConfig->IsChasing == false && AIController->IsInAttackingRange() == false)
                {
                    FinishLatentTask(OwnerComp, FinishFailedValue);
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

void UAITask_ChasePlayer::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
}
