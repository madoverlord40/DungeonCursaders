// Fill out your copyright notice in the Description page of Project Settings.

#include "TitlePawnController.h"
#include "../TitleScreenWidget.h"
#include "DefaultPawns/TitleScreenCameraPawn.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "GameStates/RootGameState.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Public/TimerManager.h"


void ATitlePawnController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (bDoFade && IsValid(TitleWidget))
	{
		if (bFadeIn)
		{
			FadeValue = FMath::FInterpConstantTo(FadeValue, 1.0f, DeltaTime, 1.0f);
			if (FadeValue >= 1.0f)
			{
				bDoFade = false;
				FinishFadeIn();
			}
		}
		else
		{
			FadeValue = FMath::FInterpConstantTo(FadeValue, 0.0f, DeltaTime, 1.0f);
			if (FadeValue <= 0.0f)
			{
				bDoFade = false;
				FinishFadeOut();
			}
		}
		TitleWidget->SetPanelOpacity(FadeValue);
	}
}


void ATitlePawnController::BeginPlay()
{
	Super::BeginPlay();
	
}


void ATitlePawnController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);

	CameraPawn = Cast<ATitleScreenCameraPawn>(aPawn);
	if (IsValid(TitleWidgetClass))
	{
		TitleWidget = CreateWidget<UTitleScreenWidget>(this, TitleWidgetClass);
		TitleWidget->AddToPlayerScreen();
	}
	bShowMouseCursor = true;
}

void ATitlePawnController::UnPossess()
{
	Super::UnPossess();
	CameraPawn = nullptr;
}

void ATitlePawnController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

}

void ATitlePawnController::PerformWidgetFadeIn()
{
	FadeValue = 0.0f;
	bShowMouseCursor = false;
	bDoFade = true;
	bFadeIn = true;
}

void ATitlePawnController::PerformWidgetFadeOut()
{
	bShowMouseCursor = false;
	FadeValue = 1.0f;
	bDoFade = true;
	bFadeIn = false;
}

void ATitlePawnController::FinishFadeIn()
{
	bShowMouseCursor = true;
}

void ATitlePawnController::FinishFadeOut()
{
	TWeakObjectPtr<ARootGameState> State = Cast<ARootGameState>(GetWorld()->GetGameState());

	if (State.IsValid())
	{
		State->AdvanceState();
	}
}
