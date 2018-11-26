// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Runtime/UMG/Public/Components/ProgressBar.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "PlayerMainUI.generated.h"

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UPlayerMainUI : public UUserWidget
{
	GENERATED_BODY()

public:
	UPlayerMainUI(const FObjectInitializer & ObjectInitializer = FObjectInitializer::Get());

	void SetHealthPercent(const float health);
	void SetStaminaPercent(const float stamina);
	void SetManaPercent(const float mana);
	void SetRagePercent(const float rage);
    void SetCharacterName(const FName& Name);
    void SetExpValue(const float exp);
    void SetLevelValue(const uint8 level);

protected:

	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProgressBar* HealthProgBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProgressBar* StaminaProgBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProgressBar* ManaProgBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProgressBar* RageProgBar;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* NameTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* LevelTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* ExpTextBlock;

private:
    bool DoUpdate;
    float NewHealthValue;
    float NewStaminaValue;
    float NewManaValue;
    float NewRageValue;
};
