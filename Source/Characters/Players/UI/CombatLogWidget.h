// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CombatLogWidget.generated.h"

class UScrollBox;
class URichTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UCombatLogWidget : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetText(const FText& NewText);
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UScrollBox * ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URichTextBlock* RichText;

};
