// Fill out your copyright notice in the Description page of Project Settings.

#include "TitleScreenWidget.h"
#include "GameStates/RootGameState.h"
#include "Runtime/UMG/Public/Components/CanvasPanel.h"
#include "DefaultPawns/TitlePawnController.h"

void UTitleScreenWidget::ExitGame()
{
	TWeakObjectPtr<ARootGameState> GameState = Cast<ARootGameState>(GetWorld()->GetGameState());
	if (GameState.IsValid())
	{
		GameState->ShutdownGame();
		if (IsValid(RootCanvasPanel))
		{
			RootCanvasPanel->SetIsEnabled(false);
		}
	}
}

void UTitleScreenWidget::StartNewGame()
{
	TWeakObjectPtr<ARootGameState> GameState = Cast<ARootGameState>(GetWorld()->GetGameState());
	if (GameState.IsValid())
	{
		GameState->SetStartGameState();
		if (IsValid(RootCanvasPanel))
		{
			RootCanvasPanel->SetIsEnabled(false);
		}
	}
}

void UTitleScreenWidget::NativePreConstruct()
{
	Super::NativePreConstruct();
}

void UTitleScreenWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UTitleScreenWidget::NativeDestruct()
{
	Super::NativeDestruct();
}

void UTitleScreenWidget::NativeTick(const FGeometry & MyGeometry, float InDeltaTime)
{
	Super::NativeTick(MyGeometry, InDeltaTime);
}

void UTitleScreenWidget::SetPanelOpacity(float value)
{
	if (IsValid(RootCanvasPanel))
	{
		RootCanvasPanel->SetRenderOpacity(value);
	}
}
