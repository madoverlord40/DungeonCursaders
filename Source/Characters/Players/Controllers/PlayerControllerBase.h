// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "../UI/PlayerWidgetHUD.h"
#include "Characters/Players/Components/AbilityManagerComponent.h"
#include "Characters/DataStructures/EquippedWeaponObject.h"
#include "Characters/Players/Components/CombatComponent.h"
#include "PlayerControllerBase.generated.h"

class APlayerCharacterBase;
class AEnemyCharacterBase;
class UItemInventoryUI;
class UInventoryComponent;
class ADungeonDoorActor;
class UDungeonDoorKey;
class APersistentLevelControler;
class UPlayerWidgetHUD;
class UCombatComponent;
class ADungeonTreasureChest;
class UEquipmentComponent;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API APlayerControllerBase : public APlayerController
{
	GENERATED_BODY()
	
	
public:
	/** Default Constructor */
	APlayerControllerBase(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	/** Pawn has been possessed, so changing state to NAME_Playing. Start it walking and begin playing with it. */
	virtual void BeginPlayingState() override;

	/** Leave playing state. */
	virtual void EndPlayingState() override;
	
	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	*/
	virtual void PlayerTick(float DeltaTime) override;

    void UpdatePlayerUI();

	virtual void Possess(APawn* aPawn) override;
	virtual void UnPossess() override;

	bool AddLootableItemToInventory(const FName& ItemName, const TSubclassOf<UItemObject> LootedItemClass);

	bool UpdateInventoryItemButtonIndex(const FName& KeyName, const uint8 index);

	void TakeSelectedLoot(const TArray<FName>& KeyNames);

	void ExitInteratingMode();

	bool UseInventoryItem(const FInventoryItemInfo& ItemInfo);

	void ShowHideDoorInteractUI(const ADungeonDoorActor* TheDoor);

	void UnlockSelectedDoor();

	void OpenSelectedDoor();

	void SetViewTarget(class AActor* NewViewTarget, FViewTargetTransitionParams TransitionParams = FViewTargetTransitionParams()) override;

	void OnNotifyToggleCombatCompleted();

	void CancelDoorInteractUI();

	bool ActivateAbility(const FAbilityInfo* Ability);

	void NotifyAttackComplete();

	void ShowHideStatsUI();

	AEnemyCharacterBase* GetCurrentTarget();

	const UAttackAbility* GetSelectedAbility() const;

	uint32 GetIsUsingCombo() const { return bUsingComboAbility; }

	const UCombatData* GetCharacterCombatData() const;

	UInventoryComponent* GetInventoryComponent() const { return InventoryComp; }

	void UpdateEnemyHealthPanel();

	bool UnEquipItem(const FInventoryItemInfo* UnequipInfo);

	bool EquipItem(const FInventoryItemInfo* UnequipInfo);

protected:

	virtual void PostInitializeComponents() override;

	/** Allows the PlayerController to set up custom input bindings. */
	virtual void SetupInputComponent() override;

	void ForwardMovement(float value);
	void RightMovement(float value);
	void MouseX(float value);
	void MouseY(float value);
    void TargetNearest();
    void TargetLastHit();
    void TargetLock();
    void StartActionJump();
    void StopActionJump();
    void ToggleCombatMode();
	void ClickOnObject();
	void ShowHideCharacterStats();

	UFUNCTION(BlueprintCallable)
	void DisplayItemInventory();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UAbilityManagerComponent* AbilityComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Components)
	UInventoryComponent* InventoryComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<UPlayerWidgetHUD> PlayerUIClass;

private:

	UPROPERTY()
	TWeakObjectPtr<UPlayerWidgetHUD> PlayerUI;

	UPROPERTY()
    TWeakObjectPtr<APlayerCharacterBase> PlayerCharacter;

	UPROPERTY()
    TWeakObjectPtr<AEnemyCharacterBase> CurrentTarget;

	UPROPERTY()
	TWeakObjectPtr<AEnemyCharacterBase> LastHitTarget;

	UPROPERTY()
	TWeakObjectPtr<ADungeonDoorActor> CurrentInteractDoor;

	UPROPERTY()
	TWeakObjectPtr<ADungeonTreasureChest> CurrentInteractChest;

	UPROPERTY()
	TWeakObjectPtr<UAttackAbility> SelectedAttackAbility;

	UPROPERTY()
	TWeakObjectPtr<UAttackAbility> SelectedRageAbility;

	UPROPERTY()
	uint32 HasTargetLocked : 1;

	FName ComboKeyName;
	uint32 bUsingComboAbility : 1;

	uint32 bIsAlive : 1;
	uint32 bIsTogglingCombat : 1;
	uint32 bIsInCombatMode : 1;
	uint32 bIsInteracting : 1;
	uint32 bIsShowingInventory : 1;
	uint32 bIsUsingAbility : 1;
	uint32 bIsShowingStatsUI : 1;

protected:
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float NearestTargetDistanceMax = 3000.0f;

};
