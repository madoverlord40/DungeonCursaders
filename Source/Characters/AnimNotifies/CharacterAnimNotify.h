// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CharacterAnimNotify.generated.h"

UENUM(BlueprintType)
enum class NotifyTypes : uint8
{
	NOTIFY_ANIM_GENERAIC,
	NOTIFY_ATTACK,
	NOTIFY_RAGEATTACK,
	NOTIFY_COLISION,
	NOTIFY_TRANSITION,
	NOTIFY_INTROANIM,
	NOTIFY_JUMP
};

UENUM(BlueprintType)
enum class NotifyTypeStates : uint8
{
	NOTIFY_STATE_START,
	NOTIFY_STATE_INPROGRESS,
	NOTIFY_STATE_COMPLETE,
	NOTIFY_STATE_ENABLED,
	NOTIFY_STATE_DISABLED
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UCharacterAnimNotify : public UAnimNotify
{
	GENERATED_BODY()
	
	
protected:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName NotificationName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	NotifyTypes NotificationType;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	NotifyTypeStates NotificationState;

};
