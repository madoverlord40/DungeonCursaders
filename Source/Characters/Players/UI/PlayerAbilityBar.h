// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/Players/Components/AbilityManagerComponent.h"
#include "PlayerAbilityBar.generated.h"

class UAbilityButton;
class UProgressBar;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerAbilityBar : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UPlayerAbilityBar(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	UFUNCTION(BlueprintCallable)
	bool RegisterAbilitySlot(UAbilityButton* AbilitySlotButton, FName SlotName);

	//enables/disables the button on the ability bar, ability will stick tick its cooldown
	UFUNCTION(BlueprintCallable)
	void SetEnableAbilitySlot(FName SlotName, bool enable);

	//enable/disable ticking of the ability and disables button
	UFUNCTION(BlueprintCallable)
	void SetEnableAbility(FName SlotName, bool enable);
	
	UFUNCTION(BlueprintCallable)
	void SetEnableAllAbilities(bool enable);
	
	UFUNCTION(BlueprintCallable)
	void SetEnableAllSlots(bool enable);

	UFUNCTION(BlueprintCallable)
	bool RegisterAbilityToSlot(FName SlotName, FAbilityInfo& AbilitySlot);

	void RegisterAbilityManager(UAbilityManagerComponent* AbilityMan, bool AutoRegisterAbilities);
	
	void OnAbilityButtonClick(const FName& SlotName);

	void UpdateExpBar(const float precent);

	void NotifyAbilityComplete();

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;

	void CheckAbilityCooldowns();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, UAbilityButton*> SlotButtons;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TMap<FName, FAbilityInfo> SlotAbilities;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAbilityManagerComponent* AbilityManager;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProgressBar* ExpPrgoressBar;

	uint32 bCoolDown : 1;

};
