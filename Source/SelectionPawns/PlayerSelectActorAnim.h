// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerSelectActorAnim.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerSelectActorAnim : public UAnimInstance
{
	GENERATED_BODY()
	
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsInitializing;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsIdle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsSelecting;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasBeenSelectedForPlay;

public:
	void SetIdleState(bool idle) { IsIdle = idle; }
	void SetIsSelecting(bool selecting) { IsSelecting = selecting; }
	void SetSelectedForPlay() { HasBeenSelectedForPlay = true; }

};
