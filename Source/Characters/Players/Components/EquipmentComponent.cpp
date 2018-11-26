// Fill out your copyright notice in the Description page of Project Settings.

#include "EquipmentComponent.h"


// Sets default values for this component's properties
UEquipmentComponent::UEquipmentComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UEquipmentComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void UEquipmentComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UEquipmentComponent::EquipRightHand(FEquipmentData* EquipData, UCombatData*& CombatData)
{
	if (EquipData->Type == ItemTypes::ITEM_EQUIPABLE_WEAPON)
	{
		if (IsValid(EquippedWeaponRight))
		{
			UnequipRightHand(CombatData);
		}

		//...
		EquippedWeaponRight = NewObject<UEquippedWeaponObject>(this, UEquippedWeaponObject::StaticClass());
		EquippedWeaponRight->CreateThis(EquipData->BattleCharacter, FName("Right Hand Weapon"), EquipData->WeaponClass);
		
		if (EquipData->bCreateMeshComponent)
		{
			EquippedWeaponRight->EquipThis(EquipData->RootMeshComp);
		}
		else
		{
			EquippedWeaponRight->EquipThis(EquipData->RootMeshComp, false);
		}
		
		EquippedWeaponRight->ConfigureThis(CombatData, false);
	}
}

void UEquipmentComponent::UnequipRightHand(UCombatData*& CombatData)
{
	EquippedWeaponRight->UnEquipThis(CombatData);
	EquippedWeaponRight->ConditionalBeginDestroy();
	EquippedWeaponRight = nullptr;
}

void UEquipmentComponent::UnequipLeftHand(UCombatData*& CombatData)
{
	if (IsValid(EquippedWeaponLeft))
	{
		EquippedWeaponLeft->UnEquipThis(CombatData);
		EquippedWeaponLeft->ConditionalBeginDestroy();
		EquippedWeaponLeft = nullptr;
	}
}

void UEquipmentComponent::EquipLeftHand(FEquipmentData* EquipData, UCombatData*& CombatData)
{
	if (EquipData->Type == ItemTypes::ITEM_EQUIPABLE_WEAPON)
	{
		if (IsValid(EquippedWeaponLeft))
		{
			UnequipLeftHand(CombatData);
		}

		//...
		EquippedWeaponLeft = NewObject<UEquippedWeaponObject>(this, UEquippedWeaponObject::StaticClass());
		EquippedWeaponLeft->CreateThis(EquipData->BattleCharacter, FName("Left Hand Weapon"), EquipData->WeaponClass);

		if (EquipData->bCreateMeshComponent)
		{
			EquippedWeaponLeft->EquipThis(EquipData->RootMeshComp);
		}
		else
		{
			EquippedWeaponLeft->EquipThis(EquipData->RootMeshComp, false);
		}
		
		EquippedWeaponLeft->ConfigureThis(CombatData, true);
	}
}

bool UEquipmentComponent::EquipArmorSlot(FEquipmentData* ArmorData, UCombatData*& CombatData)
{
	if (ArmorData && ArmorData->Type == ItemTypes::ITEM_EQUIPABLE_ARMOR)
	{
		if (IsValid(ArmorData->ArmorClass) && ArmorData->ArmorSlot != ArmorSlotTypes::ARMOR_SLOT_NONE)
		{
			UnEquipArmor(CombatData, ArmorData->ArmorSlot);

			UEquippedArmorObject* NewArmor = NewObject<UEquippedArmorObject>(this);

			switch (ArmorData->ArmorSlot)
			{
			case ArmorSlotTypes::ARMOR_SLOT_HEAD:
			{
				NewArmor->CreateThis(FName("Head Armor"), ArmorData->ArmorClass);
				break;
			}
			case ArmorSlotTypes::ARMOR_SLOT_CHEST:
				NewArmor->CreateThis(FName("Chest Armor"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_LEGS:
				NewArmor->CreateThis(FName("Legs Armor"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_FEET:
				NewArmor->CreateThis(FName("Feet Armor"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_NECKLACE:
				NewArmor->CreateThis(FName("Necklace"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_BELT:
				NewArmor->CreateThis(FName("Belt"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_RING_ONE:
				NewArmor->CreateThis(FName("Ring Slot One"), ArmorData->ArmorClass);
				break;
			case ArmorSlotTypes::ARMOR_SLOT_RING_TWO:
				NewArmor->CreateThis(FName("Ring Slot Two"), ArmorData->ArmorClass);
				break;
			default:
				break;
			}

			NewArmor->ConfigureThis(CombatData);

			if (IsValid(ArmorData->ArmorMeshComp))
			{
				NewArmor->EquipThis(ArmorData->RootMeshComp, ArmorData->ArmorMeshComp);
			}
			else if (ArmorData->bCreateMeshComponent)
			{
				NewArmor->EquipThis(ArmorData->RootMeshComp);
			}

			EquippedArmors.Add(ArmorData->ArmorSlot, NewArmor);
			return true;
		}
	}

	return false;
}

const UEquippedArmorObject* const * UEquipmentComponent::GetArmorAtSlot(ArmorSlotTypes ArmorSlot) const
{
	return EquippedArmors.Find(ArmorSlot);
}

bool UEquipmentComponent::UnEquipArmor(UCombatData*& CombatData, ArmorSlotTypes ArmorSlot)
{
	if (EquippedArmors.Find(ArmorSlot) != nullptr)
	{
		UEquippedArmorObject* Armor = EquippedArmors.FindAndRemoveChecked(ArmorSlot);
		Armor->UnEquipThis(CombatData);
		Armor->ConditionalBeginDestroy();
		Armor = nullptr;

		return true;
	}

	return false;
}

bool UEquipmentComponent::UnEquipWeapon(UCombatData*& CombatData, bool IsLeftHand)
{
	if (IsLeftHand)
	{
		UnequipLeftHand(CombatData);
	}
	else
	{
		UnequipRightHand(CombatData);
	}

	return true;
}

void UEquipmentComponent::SetWeaponCollisionEnabled(ECollisionEnabled::Type Enabled)
{
	if (IsValid(EquippedWeaponLeft))
	{
		EquippedWeaponLeft->SetCollisionEnabled(Enabled);
	}

	if (IsValid(EquippedWeaponRight))
	{
		EquippedWeaponRight->SetCollisionEnabled(Enabled);
	}
}

void UEquipmentComponent::SetEnableHighlightAll(bool enable)
{
	if (IsValid(EquippedWeaponLeft))
	{
		EquippedWeaponLeft->SetEnableHighlight(enable);
	}

	if (IsValid(EquippedWeaponRight))
	{
		EquippedWeaponRight->SetEnableHighlight(enable);
	}

	for (TPair<ArmorSlotTypes, UEquippedArmorObject*>& Pairs : EquippedArmors)
	{
		if (IsValid(Pairs.Value))
		{
			Pairs.Value->SetEnableHighlightMesh(enable);
		}
	}
}

const TMap<ArmorSlotTypes, UEquippedArmorObject*>& UEquipmentComponent::GetEquippedArmors() const
{
	return EquippedArmors;
}

