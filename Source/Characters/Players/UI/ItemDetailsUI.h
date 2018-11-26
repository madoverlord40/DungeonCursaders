// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ItemDetailsUI.generated.h"

class UTextBlock;
class UItemInventoryUI;
class UButton;
class URichTextBlock;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UItemDetailsUI : public UUserWidget
{
	GENERATED_BODY()
	
	
public:

	UFUNCTION(BlueprintCallable)
	void SetDetailsText(const FText& Text, const FText& Title, bool useable);
	
	UFUNCTION(BlueprintCallable)
	void DeleteOwningItem();

	UFUNCTION(BlueprintCallable)
	void UseThisItem();

	TWeakObjectPtr<UItemInventoryUI> InventoryWindow;


protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URichTextBlock* DetailsTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* TitleTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButton* UseThisButton;

};
