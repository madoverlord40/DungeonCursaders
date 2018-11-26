// Fill out your copyright notice in the Description page of Project Settings.

#include "ActionVoicesComponent.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundBase.h"

// Sets default values for this component's properties
UActionVoicesComponent::UActionVoicesComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

    AudioComponent = CreateDefaultSubobject<UAudioComponent>(TEXT("Audio Component"));
   
}


// Called when the game starts
void UActionVoicesComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
    PlaySpawnInVoice();
}


// Called every frame
void UActionVoicesComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

void UActionVoicesComponent::PlayRandomEmotion()
{
    int randmax = RandomEmotionVoices.Num() - 1;
    int random = FMath::RandRange(0, randmax);

    SelectedVoice = RandomEmotionVoices[random];

    if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
        PlaySelectedVoice();
    }
}

void UActionVoicesComponent::PlayRandomBattleEmotion()
{
    int randmax = InBattleEmotionVoices.Num() - 1;
    int random = FMath::RandRange(0, randmax);

    SelectedVoice = InBattleEmotionVoices[random];

    if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
        PlaySelectedVoice();
    }
}

void UActionVoicesComponent::PlaySpawnInVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(SpawnInVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayStaminaLowVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(StaminaLowVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayHealthLowVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(HealthLowVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayCantUseVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(CanNotUseVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayCantUseAbilityVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(AbilityNotReadyVoice);

        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayTakeCriticalHitVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(TakeCriticalHitVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayDeliverCriticalhitVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(DeliverCriticalHitVoice);
        
        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}

void UActionVoicesComponent::PlayRageReadyVoice()
{
    if (ActionVoiceMap.Num() > 0) {
        SelectedVoice = *ActionVoiceMap.Find(RageAbilityReadyVoice);

        if (SelectedVoice.IsValid() && SelectedVoice->IsValidLowLevelFast()) {
            PlaySelectedVoice();
        }
    }
}


void UActionVoicesComponent::PlaySelectedVoice()
{
    if (!AudioComponent->IsPlaying() && SelectedVoice.IsValid()) {
        AudioComponent->SetSound(SelectedVoice.Get());
        AudioComponent->Play();
    }
}

