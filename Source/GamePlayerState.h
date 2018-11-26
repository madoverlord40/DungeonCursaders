// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "GamePlayerState.generated.h"

class ATitleScreenCameraPawn;
class ATitlePawnController;
class ACharacterSelectPawn;
class ACharSelectPawnController;
class APlayerCharacterBase;
class APlayerControllerBase;
class APersistentGameMode;

UENUM()
enum PlayerFlowStates
{
	INITIALIZESTATE,
	TITLESCREENSTATE,
	CHARSELECTSTATE,
	GAMEPLAYSTATE,
	SHUTDOWNSTATE
};
/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API AGamePlayerState : public APlayerState
{
	GENERATED_BODY()
	
	
public:

	virtual void PostInitializeComponents() override;
	
	/** Called by Controller when its PlayerState is initially replicated. */
	virtual void ClientInitialize(class AController* C);

	void SetCurrentPlayerState(PlayerFlowStates NewState);
	



protected:

	

	PlayerFlowStates CurrentPlayerState;
};
