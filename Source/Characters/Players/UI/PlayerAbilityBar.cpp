// Fill out your copyright notice in the Description page of Project Settings.

#include "PlayerAbilityBar.h"
#include "Characters/Players/UI/AbilityButton.h"
#include "Characters/Players/Controllers/PlayerControllerBase.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"

UPlayerAbilityBar::UPlayerAbilityBar(FObjectInitializer const & ObjectInitializer) : Super(ObjectInitializer)
{
	bCoolDown = false;
}

bool UPlayerAbilityBar::RegisterAbilitySlot(UAbilityButton * AbilitySlotButton, FName SlotName)
{
	if (SlotButtons.Find(SlotName))
	{
		SlotButtons.Emplace(SlotName, AbilitySlotButton);
	}
	else
	{
		SlotButtons.Add(SlotName, AbilitySlotButton);
	}

	return true;
}

void UPlayerAbilityBar::SetEnableAbilitySlot(FName SlotName, bool enable)
{
	UAbilityButton* button = *SlotButtons.Find(SlotName);

	if (IsValid(button))
	{
		button->SetIsEnabled(enable);
	}
}

void UPlayerAbilityBar::SetEnableAbility(FName SlotName, bool enable)
{
	FAbilityInfo* Ability = SlotAbilities.Find(SlotName);

	if (Ability)
	{
		Ability->bIsEnabled = enable;
		SetEnableAbilitySlot(SlotName, enable);
	}
}

void UPlayerAbilityBar::SetEnableAllAbilities(bool enable)
{
	for (TPair<FName, UAbilityButton*> Pair : SlotButtons)
	{
		if (IsValid(Pair.Value))
		{
			SetEnableAbility(Pair.Key, enable);
		}
	}
}

void UPlayerAbilityBar::SetEnableAllSlots(bool enable)
{
	for (TPair<FName, UAbilityButton*> Pair : SlotButtons)
	{
		if (IsValid(Pair.Value))
		{
			SetEnableAbilitySlot(Pair.Key, enable);
		}
	}
}

bool UPlayerAbilityBar::RegisterAbilityToSlot(FName SlotName, FAbilityInfo& AbilitySlot)
{
	UAbilityButton* button = *SlotButtons.Find(SlotName);

	if (IsValid(button) && !SlotAbilities.Find(SlotName))
	{
		button->ParentUI = this;
		button->AbilitySlotName = SlotName;
		FButtonStyle Style = button->WidgetStyle;
		Style.Normal.SetResourceObject(AbilitySlot.IconTexture);
		Style.Normal.DrawAs = ESlateBrushDrawType::Image;
		Style.Normal.TintColor = FLinearColor(0.5f, 0.5f, 0.5f, 1.0f);
		
		Style.Hovered = Style.Normal;
		Style.Hovered.TintColor = FLinearColor(1.0f, 1.0f, 1.0f, 1.0f);
		button->SetStyle(Style);

		SlotAbilities.Add(SlotName, AbilitySlot);
		return true;
	}

	return false;
}

void UPlayerAbilityBar::RegisterAbilityManager(UAbilityManagerComponent * AbilityMan, bool AutoRegisterAbilities)
{
	AbilityManager = AbilityMan;

	if (IsValid(AbilityManager) && AutoRegisterAbilities)
	{
		FAbilityInfo NewAbility;

		const TMap<FName, FAbilityInfo>* EquippedAbilities = AbilityManager->GetEquippedAbilities();

		if (EquippedAbilities)
		{
			TArray<FName> ButtonSlotNames;
			SlotButtons.GenerateKeyArray(ButtonSlotNames);
			uint8 Index = 0;

			for (const TPair<FName, FAbilityInfo> Pair : *EquippedAbilities)
			{
				if (Index < ButtonSlotNames.Num())
				{
					const FName KeyName = Pair.Key;
					const FAbilityInfo* AbilityData = &Pair.Value;

					FAbilityInfo NewAbility;
					NewAbility.AbilityName = AbilityData->AbilityName;
					NewAbility.bIsEnabled = AbilityData->bIsEnabled;
					NewAbility.CooldownTime = AbilityData->CooldownTime;
					NewAbility.IconTexture = AbilityData->IconTexture;
					NewAbility.CurrentTime = 0.0f;
					NewAbility.KeyName = KeyName;

					FName& SlotName = ButtonSlotNames[Index];
					RegisterAbilityToSlot(SlotName, NewAbility);
					Index++;
				}
			}
		}

	}
}

void UPlayerAbilityBar::OnAbilityButtonClick(const FName& SlotName)
{
	APlayerController* player = GetOwningPlayer();

	APlayerControllerBase* PlayerController = Cast<APlayerControllerBase>(player);

	if (IsValid(PlayerController))
	{
		FAbilityInfo* AbilityInfo = SlotAbilities.Find(SlotName);
		
		if (PlayerController->ActivateAbility(AbilityInfo))
		{
			SetEnableAllSlots(false);
			SetEnableAbility(SlotName, false);
			bCoolDown = true;
		}
		
	}
}

void UPlayerAbilityBar::UpdateExpBar(const float precent)
{
	if (IsValid(ExpPrgoressBar))
	{
		ExpPrgoressBar->SetPercent(precent);
	}
}

void UPlayerAbilityBar::NotifyAbilityComplete()
{
	bCoolDown = false;
	CheckAbilityCooldowns();
}

void UPlayerAbilityBar::NativeConstruct()
{
	Super::NativeConstruct();
}

void UPlayerAbilityBar::NativeDestruct()
{
	Super::NativeDestruct();

}

void UPlayerAbilityBar::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
	//tick cooldowns

	if (!bCoolDown)
	{
		for (TPair<FName, FAbilityInfo>& Pair : SlotAbilities)
		{
			if (Pair.Value.bIsEnabled == false)
			{
				if (Pair.Value.CurrentTime > Pair.Value.CooldownTime)
				{
					Pair.Value.CurrentTime = 0.0f;
					Pair.Value.bIsEnabled = true;
					SetEnableAbilitySlot(Pair.Key, true);
				}
				else
				{
					Pair.Value.CurrentTime += InDeltaTime;
				}
			}

		}
	}
}

void UPlayerAbilityBar::CheckAbilityCooldowns()
{
	if (!bCoolDown)
	{
		for (TPair<FName, FAbilityInfo>& Pair : SlotAbilities)
		{
			if (Pair.Value.bIsEnabled)
			{
				SetEnableAbilitySlot(Pair.Key, true);
			}

		}
	}
}
