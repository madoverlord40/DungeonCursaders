// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "ActionVoicesComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class DUNGEONCRUSADES_API UActionVoicesComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UActionVoicesComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

    void PlayRandomEmotion();

    void PlayRandomBattleEmotion();

    void PlaySpawnInVoice();

    void PlayStaminaLowVoice();

    void PlayHealthLowVoice();

    void PlayCantUseVoice();

    void PlayCantUseAbilityVoice();

    void PlayTakeCriticalHitVoice();

    void PlayDeliverCriticalhitVoice();

    void PlayRageReadyVoice();

protected:
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class UAudioComponent* AudioComponent;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Audio Map")
    TMap<FName, USoundBase*> ActionVoiceMap;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Audio Map")
    TArray < USoundBase*> RandomEmotionVoices;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Audio Map")
    TArray < USoundBase*> InBattleEmotionVoices;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Voice Name Mappings")
    FName SpawnInVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName DisabledVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName StaminaLowVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName HealthLowVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName ManaLowVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName RageAbilityReadyVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName AbilityNotReadyVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName CanNotUseVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName DeathVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName TakeCriticalHitVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName DeliverCriticalHitVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName VictoryVoice;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Voice Name Mappings")
    FName LevelUpVoice;

private:
     TWeakObjectPtr<USoundBase> SelectedVoice;

    void PlaySelectedVoice();
};
