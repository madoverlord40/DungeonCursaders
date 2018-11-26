// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/Border.h"
#include "Components/Widget.h"
#include "Characters/DataStructures/InventoryItemObject.h"
#include "UsableItemButton.generated.h"

class UItemInventoryUI;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UUsableItemButton : public UUserWidget
{
	GENERATED_BODY()
	
	
public:
	UUsableItemButton(const FObjectInitializer& Initializer = FObjectInitializer::Get());

	virtual void NativeOnMouseEnter(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnMouseLeave(const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual FReply NativeOnMouseButtonUp(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	virtual void NativeOnDragDetected(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent, UDragDropOperation*& OutOperation) override;
	virtual void NativeOnDragEnter(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragLeave(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDragOver(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual bool NativeOnDrop(const FGeometry& InGeometry, const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;
	virtual void NativeOnDragCancelled(const FDragDropEvent& InDragDropEvent, UDragDropOperation* InOperation) override;

	void SetItemInfo(const FInventoryItemInfo* Info, UUserWidget* ParentWidget, uint32 CanDelete = false);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* ParentWidgetUI;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint8 ButtonIndex;

protected:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UBorder* ButtonBorder;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* NormalImage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UTexture2D* UnAssignedImage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UDragDropOperation> DragOpClass;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSubclassOf<UUsableItemButton> DragWidgetClass;

	UPROPERTY()
	UInventoryItemObject* ItemInfo;


};
