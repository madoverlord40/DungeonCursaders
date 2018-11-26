// Fill out your copyright notice in the Description page of Project Settings.

#include "AITask_PerformAttack.h"
#include "../EnemyAIControllerBase.h"

UAITask_PerformAttack::UAITask_PerformAttack()
{
    bCreateNodeInstance = true;
    bNotifyTick = true;
    bNotifyTaskFinished = true;
    bIgnoreRestartSelf = false;
    bCanUseTick = true;

}

EBTNodeResult::Type UAITask_PerformAttack::ExecuteTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    EBTNodeResult::Type Result = FinishFailedValue;

    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

    if (AIController.IsValid())
    {
        const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
        if (AIConfig != nullptr)
        {
            if (AIConfig->HasTargetSighted)
            {
                if (AIController->PerformAttackTarget())
                {
                    bCanUseTick = true;
                    Result = EBTNodeResult::InProgress;
                    
                }
            }
        }
    }

    return Result;
}

EBTNodeResult::Type UAITask_PerformAttack::AbortTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory)
{
    return EBTNodeResult::Type();
}

void UAITask_PerformAttack::TickTask(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, float DeltaSeconds)
{
	if (bCanUseTick)
	{
		TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(OwnerComp.GetAIOwner());

		if (AIController.IsValid())
		{
			const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
			if (AIConfig != nullptr)
			{
				if (AIConfig->HasTargetSighted && AIConfig->IsAttacking == false)
				{
					FinishLatentTask(OwnerComp, FinishSuccessValue);
				}

				AIController->LookAtSensedTarget();
			}
			else
			{
				FinishLatentTask(OwnerComp, FinishFailedValue);
			}

		}
		else
		{
			FinishLatentTask(OwnerComp, FinishFailedValue);
		}
	}
}

void UAITask_PerformAttack::OnTaskFinished(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory, EBTNodeResult::Type TaskResult)
{
}
