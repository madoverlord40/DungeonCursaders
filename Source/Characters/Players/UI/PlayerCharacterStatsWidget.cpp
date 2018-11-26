// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerCharacterStatsWidget.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Characters/Players/Components/CombatComponent.h"
#include "../../DataStructures/EquippedWeaponObject.h"
#include "../../DataStructures/EquippedArmorObject.h"
#include "EquippedItemButtonUI.h"
#include "../../DataStructures/ItemObject.h"


UPlayerCharacterStatsWidget::UPlayerCharacterStatsWidget(const FObjectInitializer& Initializer) : Super(Initializer)
{

}

void UPlayerCharacterStatsWidget::SetPropertyValues(const UCombatData* CharacterStats)
{
	if (IsValid(CharacterStats))
	{
		if (IsValid(AttackPowerText))
		{
			const float attack = CharacterStats->GetExternalStats()->AttackPower;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);

			AttackPowerText->SetText(FText::FromString(ValueString));
		}

		if (IsValid(DefensePowerText))
		{
			const float attack = CharacterStats->GetExternalStats()->DefensePower;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);
			DefensePowerText->SetText(FText::FromString(ValueString));
		}

		if (IsValid(HitAccText))
		{
			const float attack = CharacterStats->GetExternalStats()->AttackAcc;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);
			HitAccText->SetText(FText::FromString(ValueString));

		}

		if (IsValid(MagAttackPowerText))
		{
			const float attack = CharacterStats->GetExternalStats()->MagicPower;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);
			MagAttackPowerText->SetText(FText::FromString(ValueString));
		}

		if (IsValid(MagicDefPowerText))
		{
			const float attack = CharacterStats->GetExternalStats()->MagicDefense;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);
			MagicDefPowerText->SetText(FText::FromString(ValueString));
		}

		if (IsValid(EvasionText))
		{
			const float attack = CharacterStats->GetExternalStats()->Evasion;
			float value = FMath::FloorToFloat(attack);
			FString ValueString = FString::SanitizeFloat(value, 0);
			EvasionText->SetText(FText::FromString(ValueString));
		}
	}
}

void UPlayerCharacterStatsWidget::SetArmorSlot(ArmorSlotTypes ArmorSlot, const UEquippedArmorObject * Armor)
{
	if (IsValid(Armor))
	{
		FInventoryItemInfo ItemInfo;
		ItemInfo.ArmorSlotType = ArmorSlot;
		ItemInfo.bIsUseable = false;
		ItemInfo.ItemClass = Armor->GetArmorObject()->GetClass();
		ItemInfo.ItemDescription = Armor->GetArmorObject()->GetDescriptionString();
		ItemInfo.ItemIconTexture = Armor->GetArmorObject()->InventoryIconTexture;
		ItemInfo.ItemName = Armor->GetArmorObject()->ItemName;
		ItemInfo.Type = ItemTypes::ITEM_EQUIPABLE_ARMOR;
		ItemInfo.ArmorSlotType = Armor->GetArmorSlot();

		switch (ArmorSlot)
		{
		case ArmorSlotTypes::ARMOR_SLOT_NONE:
			break;
		case ArmorSlotTypes::ARMOR_SLOT_HEAD:
			if (IsValid(HeadSlotButton))
			{
				HeadSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_CHEST:
			if (IsValid(ChestSlotButton))
			{
				ChestSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_LEGS:
			if (IsValid(LegsSlotButton))
			{
				LegsSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_FEET:
			if (IsValid(FeerSlotButton))
			{
				FeerSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_NECKLACE:
			if (IsValid(NecklaceSlotButton))
			{
				NecklaceSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_BELT:
			if (IsValid(BeltSlotButton))
			{
				BeltSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_RING_ONE:
			if (IsValid(AccOneSlotButton))
			{
				AccOneSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		case ArmorSlotTypes::ARMOR_SLOT_RING_TWO:
			if (IsValid(AccTwoSlotButton))
			{
				AccTwoSlotButton->SetButtonInfo(&ItemInfo, this, false);
			}
			break;
		default:
			break;
		}
	}
}

void UPlayerCharacterStatsWidget::SetRightWeaponSlot(const UEquippedWeaponObject * Weapon)
{
	if (IsValid(RightWeaponSlot) && IsValid(Weapon))
	{
		FInventoryItemInfo ItemInfo;
		//ItemInfo.ArmorSlotType = Slot;
		ItemInfo.bIsUseable = false;
		ItemInfo.ItemClass = Weapon->GetWeaponObject()->GetClass();
		ItemInfo.ItemDescription = Weapon->GetWeaponObject()->GetDescriptionString();
		ItemInfo.ItemIconTexture = Weapon->GetWeaponObject()->InventoryIconTexture;
		ItemInfo.ItemName = Weapon->GetWeaponObject()->ItemName;
		ItemInfo.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;

		RightWeaponSlot->SetButtonInfo(&ItemInfo, this, false);
	}
}

void UPlayerCharacterStatsWidget::SetLeftWeaponSlot(const UEquippedWeaponObject * Weapon)
{
	if (IsValid(LeftWeaponSlot) && IsValid(Weapon))
	{
		FInventoryItemInfo ItemInfo;
		//ItemInfo.ArmorSlotType = Slot;
		ItemInfo.bIsUseable = false;
		ItemInfo.ItemClass = Weapon->GetWeaponObject()->GetClass();
		ItemInfo.ItemDescription = Weapon->GetWeaponObject()->GetDescriptionString();
		ItemInfo.ItemIconTexture = Weapon->GetWeaponObject()->InventoryIconTexture;
		ItemInfo.ItemName = Weapon->GetWeaponObject()->ItemName;
		ItemInfo.Type = ItemTypes::ITEM_EQUIPABLE_WEAPON;

		LeftWeaponSlot->SetButtonInfo(&ItemInfo, this, false);
	}
}


void UPlayerCharacterStatsWidget::NativeConstruct()
{
	Super::NativeConstruct();

}

void UPlayerCharacterStatsWidget::NativeDestruct()
{
	Super::NativeDestruct();

}

void UPlayerCharacterStatsWidget::NativeTick(const FGeometry& MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);

}
