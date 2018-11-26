// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Decorators/BTDecorator_ConditionalLoop.h"
#include "AIDecorator_HasTargetLoop.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UAIDecorator_HasTargetLoop : public UBTDecorator_ConditionalLoop
{
	GENERATED_BODY()
	
public:
    UAIDecorator_HasTargetLoop();

    /** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
    virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
    virtual void OnGameplayTaskDeactivated(UGameplayTask& Task) override {}
	
    virtual bool CalculateRawConditionValue(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) const override;
    virtual EBlackboardNotificationResult OnBlackboardKeyValueChange(const UBlackboardComponent& Blackboard, FBlackboard::FKey ChangedKeyID) override;
    virtual void OnNodeDeactivation(FBehaviorTreeSearchData& SearchData, EBTNodeResult::Type NodeResult) override;

};
