// Fill out your copyright notice in the Description page of Project Settings.

#include "AIDecorator_HasTargetLoop.h"
#include "BehaviorTree/BTCompositeNode.h"
#include "../EnemyAIControllerBase.h"

UAIDecorator_HasTargetLoop::UAIDecorator_HasTargetLoop() : Super()
{
    NodeName = "Has Target Sighted Loop";
    bNotifyDeactivation = true;

    bAllowAbortNone = false;
    bAllowAbortLowerPri = true;
    bAllowAbortChildNodes = true;
}

bool UAIDecorator_HasTargetLoop::CalculateRawConditionValue(UBehaviorTreeComponent & OwnerComp, uint8 * NodeMemory) const
{
    return Super::CalculateRawConditionValue(OwnerComp, NodeMemory);
}

EBlackboardNotificationResult UAIDecorator_HasTargetLoop::OnBlackboardKeyValueChange(const UBlackboardComponent & Blackboard, FBlackboard::FKey ChangedKeyID)
{
    return Super::OnBlackboardKeyValueChange(Blackboard, ChangedKeyID);
}

void UAIDecorator_HasTargetLoop::OnNodeDeactivation(FBehaviorTreeSearchData & SearchData, EBTNodeResult::Type NodeResult)
{
    TWeakObjectPtr<AEnemyAIControllerBase> AIController = Cast<AEnemyAIControllerBase>(SearchData.OwnerComp.GetAIOwner());

    if (NodeResult != EBTNodeResult::Aborted)
    {
        if (AIController.IsValid())
        {
            const FAIConfigurationData* AIConfig = AIController->GetAIConfig();
            if (AIConfig != nullptr)
            {
                if (AIConfig->HasTargetSighted)
                {
                    GetParentNode()->SetChildOverride(SearchData, GetChildIndex());
                }
            }
        }
        
    }
}
