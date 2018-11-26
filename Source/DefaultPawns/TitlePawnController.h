// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "TitlePawnController.generated.h"

class UTitleScreenWidget;
class ATitleScreenCameraPawn;

/**
 * 
 */
UCLASS()
class DUNGEONCRUSADES_API ATitlePawnController : public APlayerController
{
	GENERATED_BODY()
	
protected:
 	UPROPERTY(EditAnywhere)
 	TSubclassOf<UTitleScreenWidget> TitleWidgetClass;

	/**
	* Processes player input (immediately after PlayerInput gets ticked) and calls UpdateRotation().
	* PlayerTick is only called if the PlayerController has a PlayerInput object. Therefore, it will only be called for locally controlled PlayerControllers.
	*/
	virtual void PlayerTick(float DeltaTime) override;
	
	/** Pawn has been possessed, so changing state to NAME_Playing. Start it walking and begin playing with it. */
	virtual void BeginPlay() override;

	virtual void PostInitializeComponents() override;

	void FinishFadeIn();
	void FinishFadeOut();

public:
	virtual void Possess(APawn* aPawn) override;
	virtual void UnPossess() override;
	void PerformWidgetFadeIn();
	void PerformWidgetFadeOut();


private:
	
	UTitleScreenWidget* TitleWidget;
	TWeakObjectPtr<ATitleScreenCameraPawn> CameraPawn;
	uint32 bDoFade : 1;
	uint32 bFadeIn : 1;
	float FadeValue;
};
