// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "DoorInteractWindow.generated.h"

class UTextBlock;
class URichTextBlock;
class UPlayerWidgetHUD;
class UButton;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API UDoorInteractWindow : public UUserWidget
{
	GENERATED_BODY()
	
public:

	UFUNCTION(BlueprintCallable)
	void UnlockDoor();

	UFUNCTION(BlueprintCallable)
	void OpenDoor();

	UFUNCTION(BlueprintCallable)
	void ClickedCancel();
	
	void SetButtonStatuses(bool CanUnlock, bool CanOpen);

	void SetWindowTexts(const FText& TitleText, const FText& DetailsText);

	TWeakObjectPtr<UPlayerWidgetHUD> ParentUI;

protected:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	URichTextBlock* DetailsTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UTextBlock* TitleTextBlock;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* UnlockDoorButton;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UUserWidget* OpenDoorButton;

};
