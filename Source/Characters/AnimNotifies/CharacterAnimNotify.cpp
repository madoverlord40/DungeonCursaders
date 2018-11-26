// Fill out your copyright notice in the Description page of Project Settings.

#include "CharacterAnimNotify.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "Characters/BattleCharacterBase.h"




void UCharacterAnimNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation)
{
	TWeakObjectPtr<ACharacter> OwnerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (OwnerCharacter.IsValid() && OwnerCharacter->IsValidLowLevel())
	{
		TWeakObjectPtr<ABattleCharacterBase> BattleCharacter = Cast<ABattleCharacterBase>(OwnerCharacter);

		if (BattleCharacter.IsValid() && BattleCharacter->IsValidLowLevel())
		{
			BattleCharacter->OnAnimationNotify(NotificationName, NotificationType, NotificationState);
		}
	}
}
