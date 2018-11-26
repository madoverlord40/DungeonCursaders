// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Characters/DataStructures/CombatData.h"
#include "CharacterProfileWidget.generated.h"

class UTextBlock;
class UCanvasPanel;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UCharacterProfileWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	void SetNameAndClassInfo(const FName& CharacterName, const FName& CharacterClass, const bool DualWeilding, const bool TwoHanded);
	void SetInternalInfo(const FInternalStats* Internals);
	void SetExternalInfo(const FExternalStats* Externals);
	void SetProfileCanvasVisibility(ESlateVisibility Visibility);

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UCanvasPanel* ProfileStatsPanel;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* NameText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* ClassText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* DualWeildingText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* TwoHandedText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* StrengthText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* VitalityText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* DexterityText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* AgilityText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* MindText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* IntellectText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* HPText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* AttackText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* DefenseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* EvasionText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* MagicPowerText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* MagicDefenseText;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* AttackAccText;




	virtual void NativePreConstruct() override;
	virtual void NativeConstruct() override;
	virtual void NativeDestruct() override;

	virtual void NativeTick(const FGeometry& MyGeometry, float InDeltaTime) override;
	
};
