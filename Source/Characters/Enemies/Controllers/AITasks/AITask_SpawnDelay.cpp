// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_SpawnDelay.h"
#include "../EnemyAIControllerBase.h"

UAITask_SpawnDelay::UAITask_SpawnDelay()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = false;
}

EBTNodeResult::Type UAITask_SpawnDelay::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishSuccessValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr && AIConfig->IsPerceptionActive == false)
        {
            DelayTime = AIConfig->SpawnInAIDelayTime;
            bCanUseTick = true;
            Result = EBTNodeResult::InProgress;
            
        }
        
    }
    return Result;
}

EBTNodeResult::Type UAITask_SpawnDelay::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_SpawnDelay::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
    if (bCanUseTick) 
    {
        TickTimer += DeltaSeconds;
        if (TickTimer > DelayTime)
        {
            bCanUseTick = false;
            TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

            if (AIController.IsValid())
            {
                AIController->ActivatePerception();
                FinishLatentTask(OwnerComp, FinishSuccessValue);
            }
        }
    }
}

void UAITask_SpawnDelay::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
    TickTimer = 0.0f;
}
