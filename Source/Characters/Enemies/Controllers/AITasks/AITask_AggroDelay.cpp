// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_AggroDelay.h"
#include "../EnemyAIControllerBase.h"

UAITask_AggroDelay::UAITask_AggroDelay()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = false;
}

EBTNodeResult::Type UAITask_AggroDelay::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishFailedValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr && AIConfig->HasTargetSighted)
        {
            DelayTime = AIConfig->AggroDelay;
            bCanUseTick = true;
            Result = EBTNodeResult::InProgress;

        }

    }
    return Result;
}

EBTNodeResult::Type UAITask_AggroDelay::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_AggroDelay::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    if (bCanUseTick)
    {
        TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());
        if (AIController.IsValid())
        {
            const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
            if (AIConfig != nullptr)
            {
                TickTimer += DeltaSeconds;
                if (TickTimer > DelayTime && AIConfig->HasTargetSighted)
                {
                    bCanUseTick = false;
                    FinishLatentTask(OwnerComp, FinishSuccessValue);

                }
                else if (AIConfig->HasTargetSighted == false)
                {
                    bCanUseTick = false;
                    FinishLatentTask(OwnerComp, FinishFailedValue);
                }
            }
            else
            {
                bCanUseTick = false;
                FinishLatentTask(OwnerComp, FinishFailedValue);
            }
        }
        else
        {
            bCanUseTick = false;
            FinishLatentTask(OwnerComp, FinishFailedValue);
        }
    }
}

void UAITask_AggroDelay::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
    TickTimer = 0.0f;
}
