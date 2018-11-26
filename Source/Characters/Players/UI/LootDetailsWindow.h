// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootDetailsWindow.generated.h"

class URichTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ULootDetailsWindow : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemDetails(const FText& Details);
	
protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URichTextBlock* DetailsTextBox;
	
	
};
