// Fill out your copyright notice in the Description page of Project Settings.

#include "DoorInteractWindow.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "PlayerWidgetHUD.h"
#include "Runtime/UMG/Public/Components/Button.h"
#include "Runtime/UMG/Public/Components/TextBlock.h"
#include "Runtime/UMG/Public/Components/RichTextBlock.h"

void UDoorInteractWindow::UnlockDoor()
{
	if (ParentUI.IsValid())
	{
		ParentUI->UnlockDoor();
	}
}

void UDoorInteractWindow::OpenDoor()
{
	if (ParentUI.IsValid())
	{
		ParentUI->OpenDoor();
	}
}

void UDoorInteractWindow::ClickedCancel()
{
	if (ParentUI.IsValid())
	{
		ParentUI->ShowDoorInteractWindow(false, false, FText(), FText());
	}
}

void UDoorInteractWindow::SetButtonStatuses(bool CanUnlock, bool CanOpen)
{
	if (IsValid(OpenDoorButton))
	{
		OpenDoorButton->SetIsEnabled(CanOpen);
	}

	if (IsValid(UnlockDoorButton))
	{
		UnlockDoorButton->SetIsEnabled(CanUnlock);
	}
}

void UDoorInteractWindow::SetWindowTexts(const FText& TitleText, const FText& DetailsText)
{
	if (IsValid(TitleTextBlock))
	{
		TitleTextBlock->SetText(TitleText);
	}

	if (IsValid(DetailsTextBlock))
	{
		DetailsTextBlock->SetText(DetailsText);
	}
}
