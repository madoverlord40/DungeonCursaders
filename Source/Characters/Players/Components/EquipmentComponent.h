// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Characters/DataStructures/EquippedArmorObject.h"
#include "Characters/DataStructures/EquippedWeaponObject.h"
#include "Runtime/Engine/Classes/Components/SkeletalMeshComponent.h"
#include "EquipmentComponent.generated.h"

class UEquippedWeaponObject;
class UEquippedArmorObject;
class ABattleCharacterBase;

struct FEquipmentData
{
	USkeletalMeshComponent* RootMeshComp;
	USkeletalMeshComponent* ArmorMeshComp;
	UStaticMeshComponent* WeaponMeshComp;
	ABattleCharacterBase* BattleCharacter;
	ArmorSlotTypes ArmorSlot;
	ItemTypes Type;
	TSubclassOf<UWeaponObject> WeaponClass;
	TSubclassOf<UArmorObject> ArmorClass;
	uint32 bCreateMeshComponent : 1;
};


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API UEquipmentComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	UEquipmentComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void EquipRightHand(FEquipmentData* EquipData, UCombatData*& CombatData);
	void EquipLeftHand(FEquipmentData* EquipData, UCombatData*& CombatData);
	bool EquipArmorSlot(FEquipmentData* ArmorData, UCombatData*& CombatData);

	const UEquippedArmorObject* const * GetArmorAtSlot(ArmorSlotTypes ArmorSlot) const;

	bool UnEquipArmor(UCombatData*& CombatData, ArmorSlotTypes ArmorSlot);

	bool UnEquipWeapon(UCombatData*& CombatData, bool IsLeftHand = false);

	void SetWeaponCollisionEnabled(ECollisionEnabled::Type Enabled);

	void SetEnableHighlightAll(bool enable);

	const UEquippedWeaponObject* GetEquippedRightWeapon() const { return EquippedWeaponRight; }

	const UEquippedWeaponObject* GetEquippedLeftWeapon() const { return EquippedWeaponLeft; }

	const TMap<ArmorSlotTypes, UEquippedArmorObject*>& GetEquippedArmors() const;
private:

	void UnequipRightHand(UCombatData*& CombatData);
	void UnequipLeftHand(UCombatData*& CombatData);

	UPROPERTY()
	UEquippedWeaponObject* EquippedWeaponLeft;

	UPROPERTY()
	UEquippedWeaponObject* EquippedWeaponRight;

	UPROPERTY()
	TMap<ArmorSlotTypes, UEquippedArmorObject*> EquippedArmors;

};
