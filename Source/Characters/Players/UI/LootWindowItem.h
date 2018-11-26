// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LootWindowItem.generated.h"


class UTextBlock;
class UTexture2D;
class UImage;
class UBorder;
class UTreasureLootWindow;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ULootWindowItem : public UUserWidget
{
	GENERATED_BODY()
	

public:
	void SetItemInfo(const FName& KeyID, UTreasureLootWindow* ParentWindow, const FText& text, UTexture2D* IconImage);

	UFUNCTION(BlueprintCallable)
	void NotifyCheckChanged(bool bIsChecked) { bIsThisItemChecked = bIsChecked; }

	UFUNCTION(BlueprintCallable)
	bool GetIsItemChecked() const { return bIsThisItemChecked; }

	const FName& GetItemID() const { return ItemKeyID; }

	void SetEnableSelection(bool enable);

	bool GetIsSelected() const { return bIsSelected; }

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UBorder* ItemBorder;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UTextBlock* ItemText;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UImage* ItemIcon;

	uint32 bIsThisItemChecked : 1;
	FName ItemKeyID;

	uint32 bIsSelected : 1;

	UPROPERTY()
	TWeakObjectPtr<UTreasureLootWindow> ParentUI;
	
};
