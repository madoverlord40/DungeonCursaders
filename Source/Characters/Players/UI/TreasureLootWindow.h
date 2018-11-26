// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "TreasureLootWindow.generated.h"

class ULootWindowItem;
class UButton;
class UScrollBox;
class ULootDetailsWindow;
class UPlayerWidgetHUD;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UTreasureLootWindow : public UUserWidget
{
	GENERATED_BODY()
	
	
protected:


	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UButton* ItemDetailsButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UScrollBox* ScrollBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	TSubclassOf<ULootWindowItem> LootWindowItemClass;


public:
	void SetCurrentSelectedItem(FName& ID);

	void AddLootItemToWindow(const FName& KeyName, const FText& text, UTexture2D* IconImage, const FString& Details);

	UFUNCTION(BlueprintCallable)
	void SetDetailsWindowPtr(ULootDetailsWindow* Window);

	UFUNCTION(BlueprintCallable)
	void SetParentWindowPtr(UPlayerWidgetHUD* Window);

	UFUNCTION(BlueprintCallable)
	void ShowDetailsForSelectedItem();

	UFUNCTION(BlueprintCallable)
	void OnNotifyTakeSelectedItems();

	UFUNCTION(BlueprintCallable)
	void OnNotifyTakeAllItems();

	UFUNCTION(BlueprintCallable)
	void OnNotifyCloseWindow();

private:
	FName CurrentSelectedID;

	UPROPERTY()
	TMap<FName, ULootWindowItem*> LootWindowItems;

	UPROPERTY()
	TMap<FName, FString> LootWindowDetails;

	UPROPERTY()
	ULootWindowItem* LastSelectedItem;

	UPROPERTY()
	ULootDetailsWindow* DetailsWindow;

	UPROPERTY()
	UPlayerWidgetHUD* ParentUI;

};
