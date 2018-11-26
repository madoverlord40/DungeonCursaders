// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DataStructures/ItemObject.h"
#include "PlayerCharacterStatsWidget.generated.h"

class UTextBlock;
class UCombatData;
class UEquippedItemButtonUI;
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerCharacterStatsWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UPlayerCharacterStatsWidget(const FObjectInitializer& Initializer = FObjectInitializer::Get());

	void SetPropertyValues(const UCombatData* CharacterStats);

	void SetArmorSlot(ArmorSlotTypes Slot, const class UEquippedArmorObject* Armor);
	void SetRightWeaponSlot(const class UEquippedWeaponObject* Weapon);
	void SetLeftWeaponSlot(const class UEquippedWeaponObject* Weapon);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* HeadSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* ChestSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* LegsSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* FeerSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* NecklaceSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* BeltSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* AccOneSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* AccTwoSlotButton;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* RightWeaponSlot;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UEquippedItemButtonUI* LeftWeaponSlot;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* AttackPowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* DefensePowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* HitAccText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* MagAttackPowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* MagicDefPowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* EvasionText;
};
