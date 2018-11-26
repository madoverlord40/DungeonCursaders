// Fill out your copyright notice in the Description page of Project Settings.

#include "CharSelectPawnController.h"
#include "Runtime/CinematicCamera/Public/CameraRig_Rail.h"
#include "DefaultPawns/CharacterSelectPawn.h"
#include "DefaultPawns/CharSelectPawnController.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "DungeonCrusadesInstance.h"
#include "Runtime/Engine/Public/TimerManager.h"
#include "CharacterProfileWidget.h"
#include "GameModes/CharSelectGameMode.h"
#include "GameStates/CharSelectGameState.h"

FVector ProfileViewZoomPosition;
ALevelSequenceActor * SequenceActor = nullptr;

ACharSelectPawnController::ACharSelectPawnController()
{
	
}

void ACharSelectPawnController::PlayerTick(float DeltaTime)
{
	Super::PlayerTick(DeltaTime);

	if (PerformRailMovement && MainRailRig.IsValid())
	{
		const float position = MainRailRig->CurrentPositionOnRail;

		// Distance to reach
		const float Dist = MoveToPosition - position;
		if (FMath::Square(Dist) < SMALL_NUMBER)
		{
			MainRailRig->CurrentPositionOnRail = MoveToPosition;

			SetSelectionChangeComplete();
		}
		else
		{
			const float Step = RailInterpSpeed * DeltaTime;
			const float NewPosition =  position + FMath::Clamp<float>(Dist, -Step, Step);

			// Delta Move, Clamp so we do not over shoot.
			//const float DeltaMove = Dist * FMath::Clamp<float>(DeltaTime * RailInterpSpeed, 0.f, 1.f);

			//const float NewPosition = position + DeltaMove;

			MainRailRig->CurrentPositionOnRail = NewPosition;
		}

	}
	else if (bIsPlayingSequence)
	{
		if (SequencePlayer->IsPlaying() == false)
		{
			if (GameState.IsValid())
			{
				SequenceActor->Destroy();
				bIsPlayingSequence = false;

				GameState->AdvanceState();
				if (!bSelectionChosen)
				{
					SetViewTargetWithBlend(SelectionPawn.Get(), 0.5f);
					SetSelectionChangeComplete();
				}
				
			}
		}
	}
	else if (bIsCameraFading)
	{
		if (PlayerCameraManager->FadeTimeRemaining == 0.0f)
		{
			
			if (GameState.IsValid())
			{				
				GameState->AdvanceState();				
				bIsCameraFading = false;
			}
		}
	}
	else if (bIsProfileZooming)
	{
		FVector PawnLocation = SelectionPawn->GetActorLocation();
		FVector NewLocation = FMath::VInterpConstantTo(PawnLocation, ProfileViewZoomPosition, DeltaTime, 200.0f);

		SelectionPawn->SetActorLocation(NewLocation);

		if (FMath::IsNearlyZero(FVector::Distance(PawnLocation, ProfileViewZoomPosition)))
		{
			bIsProfileZooming = false;
			if (!bInProfileView)
			{
				SelectionPawn->AttachToActor(MainRailRig.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainRailRig->GetAttachParentSocketName());
				ProfileWidget->SetVisibility(ESlateVisibility::Visible);
			}

			GameState->AdvanceState();
		}
	}
}

void ACharSelectPawnController::Possess(APawn * aPawn)
{
	Super::Possess(aPawn);
	SelectionPawn = Cast<ACharacterSelectPawn>(aPawn);
	if (SelectionPawn.IsValid())
	{
		if (MainRailRig->GetName().Contains(FString("Main")))
		{
			SelectionPawn->AttachToActor(MainRailRig.Get(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, MainRailRig->GetAttachParentSocketName());
			SelectionPawn->SetActorRelativeRotation(FRotator(0.0f, 90.0f, 0.0f));

			if (IsValid(CharacterProfileWidgetClass) && !ProfileWidget.IsValid())
			{
				ProfileWidget = CreateWidget<UCharacterProfileWidget>(this, CharacterProfileWidgetClass);
				ProfileWidget->AddToPlayerScreen();
				ProfileWidget->SetVisibility(ESlateVisibility::Collapsed);

			}
		}
	}
	
}

void ACharSelectPawnController::UnPossess()
{
	Super::UnPossess();
	SelectionPawn = nullptr;
}

void ACharSelectPawnController::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	TWeakObjectPtr<ACharSelectGameMode> GameMode = Cast<ACharSelectGameMode>(GetWorld()->GetAuthGameMode());

	if (GameMode.IsValid())
	{
		GameState = Cast<ACharSelectGameState>(GetWorld()->GetGameState());

		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACameraRig_Rail::StaticClass(), FoundActors);

		if (FoundActors.Num() > 0)
		{
			for (int32 index = 0; index < FoundActors.Num(); index++)
			{
				TWeakObjectPtr<AActor> actor = FoundActors[index];
				MainRailRig = Cast<ACameraRig_Rail>(actor);
				if (MainRailRig.IsValid())
				{
					if (MainRailRig->GetName().Contains(FString("Main")))
					{
						CharacterSelectIndex = 0;
						break;
					}
				}
			}
		}
	}
}

void ACharSelectPawnController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction(FName("SelectCharacter"), IE_Pressed, this, &ACharSelectPawnController::SelectCurrentOption);
	InputComponent->BindAxis(FName("Right"), this, &ACharSelectPawnController::MoveRightOnRail);
}

void ACharSelectPawnController::MovePawnAcrossRail(const float NewRailPosition)
{
	MoveToPosition = NewRailPosition;
	PerformRailMovement = true;
}

void ACharSelectPawnController::SelectCurrentOption()
{
	if (GameState.IsValid())
	{
		bSelectionChosen = true;
		TArray<FString> Keys;

		GameState->GetSelectionKeyNames(Keys);

		const FString& SelectedCharacterKey = Keys[CharacterSelectIndex];

		ULevelSequence* TransitionOut = *TransitionOutSequences.Find(SelectedCharacterKey);
		if (IsValid(TransitionOut))
		{
			PlaybackSettings.bRestoreState = false;

			GameState->SetCurrentSelection(SelectedCharacterKey);
			SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, TransitionOut, PlaybackSettings, SequenceActor);
			SequenceActor->bAutoPlay = true;
			SequencePlayer->Play();
			bIsPlayingSequence = true;

			SelectionPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
			SelectionPawn->SetActorLocation(FVector::ZeroVector);
			SelectionPawn->SetLookatTarget(nullptr);
		}
	}
}

void ACharSelectPawnController::MoveRightOnRail(const float amount)
{
	if (amount < 0.0f)
	{
		if (GameState.IsValid())
		{
			ChangeSelectionLeft();

		}
	}
	else if (amount > 0.0f)
	{
		if (GameState.IsValid())
		{
			ChangeSelectionRight();

		}
	}
}


void ACharSelectPawnController::ChangeSelectionLeft()
{
	if (CharacterSelectIndex > 0)
	{
		CharacterSelectIndex--;

		bSelectionChanged = true;
		DisableInput(this);

		ConfigureRailMovement();
	}
}

void ACharSelectPawnController::ChangeSelectionRight()
{
	if (CharacterSelectIndex < CharacterSelectRailPositions.Num() - 1)
	{
		CharacterSelectIndex++;

		bSelectionChanged = true;
		DisableInput(this);

		ConfigureRailMovement();
	}
}


void ACharSelectPawnController::ConfigureRailMovement()
{
	if (GameState.IsValid())
	{
		TArray<FString> Keys;

		GameState->GetSelectionKeyNames(Keys);

		const FString& SelectedCharacterKey = Keys[CharacterSelectIndex];

		GameState->SetEnablePawnLight(SelectedCharacterKey, false);
		
		RailPosition = CharacterSelectRailPositions.FindChecked(SelectedCharacterKey);

		MovePawnAcrossRail(RailPosition);

		GameState->SetCurrentSelection(SelectedCharacterKey);
	}
}

void ACharSelectPawnController::SetSelectionChangeComplete()
{
	PerformRailMovement = false;
	bSelectionChanged = false;

	if (GameState.IsValid())
	{
		TArray<FString> Keys;

		GameState->GetSelectionKeyNames(Keys);

		const FString& SelectedCharacterKey = Keys[CharacterSelectIndex];

		GameState->SetEnablePawnLight(SelectedCharacterKey, true);
		ProfileWidget->SetVisibility(ESlateVisibility::Visible);
		EnableInput(this);
		bShowMouseCursor = true;
	}
}

void ACharSelectPawnController::ExitToRootScreen()
{
	if (!bIsCameraFading)
	{
		bSelectionChosen = false;
		bIsCameraFading = true;
		ProfileWidget->SetVisibility(ESlateVisibility::Hidden);
		DisableInput(this);
		PlayerCameraManager->StartCameraFade(0.0f, 1.0f, 3.0f, FLinearColor::Black, true, true);
	}
}

void ACharSelectPawnController::ViewCharacterProfile()
{
	TArray<FString> Keys;

	GameState->GetSelectionKeyNames(Keys);

	const FString& SelectedCharacterKey = Keys[CharacterSelectIndex];

	const TWeakObjectPtr<UCombatData> PlayerBattleStats = (UCombatData*)GameState->GetCombatDataByName(SelectedCharacterKey);

	if (PlayerBattleStats.IsValid())
	{
		ProfileWidget->SetVisibility(ESlateVisibility::Visible);

		bShowMouseCursor = true;

		PlayerBattleStats->InitializeData(*SelectedCharacterKey, 1);
		const FInternalStats* Internals = PlayerBattleStats->GetInternalStats();
		const FExternalStats* Externals = PlayerBattleStats->GetExternalStats();

		ProfileWidget->SetNameAndClassInfo(PlayerBattleStats->GetCharacterName(), PlayerBattleStats->GetClassName(), PlayerBattleStats->GetIsDualWeilding(), PlayerBattleStats->GetIsTwoHanded());
		ProfileWidget->SetInternalInfo(Internals);
		ProfileWidget->SetExternalInfo(Externals);

		SelectionPawn->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		ProfileViewZoomPosition = SelectionPawn->GetActorLocation();
		ProfileViewZoomPosition += (SelectionPawn->GetActorForwardVector() * 300.0f);

		bInProfileView = true;
		bIsProfileZooming = true;
	}
}

void ACharSelectPawnController::ExitCharacterProfile()
{
	ProfileViewZoomPosition = SelectionPawn->GetActorLocation();
	ProfileViewZoomPosition -= (SelectionPawn->GetActorForwardVector() * 300.0f);

	bInProfileView = false;
	bIsProfileZooming = true;
}

void ACharSelectPawnController::PlayIntoMovie()
{
	if (IsValid(TransitionInSequence) && !bIsPlayingSequence)
	{
		SequencePlayer = ULevelSequencePlayer::CreateLevelSequencePlayer(this, TransitionInSequence, PlaybackSettings, SequenceActor);
		SequencePlayer->Play();
		bIsPlayingSequence = true;
		SelectionPawn->SetActorRelativeRotation(FRotator(0.0f, -180.0f, 0.0f));
	}
}

void ACharSelectPawnController::SetCameraLookAt(TWeakObjectPtr<APlayerSelectionActor> LookAt)
{
	if (SelectionPawn.IsValid() && LookAt.IsValid())
	{
		SelectionPawn->SetLookatTarget(LookAt);
		ProfileWidget->SetVisibility(ESlateVisibility::Visible);
	}
}

