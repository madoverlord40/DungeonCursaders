// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_IdleWait.h"
#include "../EnemyAIControllerBase.h"

UAITask_IdleWait::UAITask_IdleWait()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = false;
}

EBTNodeResult::Type UAITask_IdleWait::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishFailedValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr)
        {
            DelayTime = AIConfig->IdleWaitDelay;
            bCanUseTick = true;
            Result = EBTNodeResult::InProgress;

        }

    }
    return Result;
}

EBTNodeResult::Type UAITask_IdleWait::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_IdleWait::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
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
                if (TickTimer > DelayTime || AIConfig->HasTargetSighted)
                {
                    bCanUseTick = false;
                    FinishLatentTask(OwnerComp, FinishSuccessValue);

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

void UAITask_IdleWait::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
    TickTimer = 0.0f;
}
