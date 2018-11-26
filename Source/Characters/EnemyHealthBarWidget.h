// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EnemyHealthBarWidget.generated.h"

class UProgressBar;
class UTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UEnemyHealthBarWidget : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextBlock* LevelText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UProgressBar* HealthProgressBar;

	UPROPERTY(EditAnywhere)
	FLinearColor HighlightTextColor;

	UPROPERTY(EditAnywhere)
	FLinearColor NormalTextColor;


public:

	UFUNCTION(BlueprintCallable)
	void UpdateInfo(const FName& Name, const uint8 Level, const float HealthPercent);

	void EnableHighlightText(bool enable);
};
