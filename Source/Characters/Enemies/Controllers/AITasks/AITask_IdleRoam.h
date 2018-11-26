// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "AITask_IdleRoam.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UAITask_IdleRoam : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
    UAITask_IdleRoam();

    /** Notify called after GameplayTask changes state to Active (initial activation or resuming) */
    virtual void OnGameplayTaskActivated(UGameplayTask& Task) override {}
    /** starts this task, should return Succeeded, Failed or InProgress
    *  (use FinishLatentTask() when returning InProgress)
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

protected:
    /** aborts this task, should return Aborted or InProgress
    *  (use FinishLatentAbort() when returning InProgress)
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

    /** ticks this task
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

    /** called when task execution is finished
    * this function should be considered as const (don't modify state of object) if node is not instanced! */
    virtual void OnTaskFinished(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, EBTNodeResult::Type TaskResult) override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TEnumAsByte<EBTNodeResult::Type> FinishSuccessValue;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    TEnumAsByte<EBTNodeResult::Type> FinishFailedValue;

    uint32 bCanUseTick : 1;
	
};
