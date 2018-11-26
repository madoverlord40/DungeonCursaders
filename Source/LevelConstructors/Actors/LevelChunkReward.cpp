#include "LevelChunkReward.h"
#include "Runtime/Engine/Classes/Components/AudioComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystemComponent.h"
#include "Runtime/Engine/Classes/Sound/SoundCue.h"
#include "Runtime/Engine/Classes/Engine/World.h"
#include "Characters/Enemies/LootItems/ModifyStatsDrop.h"
#include "../WallDoorwayConstructor.h"

ALevelChunkReward::ALevelChunkReward(const FObjectInitializer& Initializer) : Super(Initializer)
{
	ParticleComponent = Initializer.CreateDefaultSubobject<UParticleSystemComponent>(this, FName("Particle System"));
	SoundComponent = Initializer.CreateDefaultSubobject<UAudioComponent>(this, FName("Sound Component"));
	MusicComponent = Initializer.CreateDefaultSubobject<UAudioComponent>(this, FName("Music Component"));
}

void ALevelChunkReward::BeginPlay()
{
	Super::BeginPlay();

	if (TypesOfRewardMap.Num() > 0)
	{
		ParticleComponent->ActivateSystem(true);
		SoundComponent->Play();
		MusicComponent->Play();

		for (const TPair<FString, RewardTypes>& Iterator : TypesOfRewardMap)
		{
			const FString& Key = Iterator.Key;
			const RewardTypes Type = Iterator.Value;

			switch (Type)
			{
			case RewardTypes::REWARD_NONE:
				break;
			case RewardTypes::REWARD_OPENDOOR:
			{
				if (WallDoorToOpen.IsValid() && WallDoorToOpen->IsValidLowLevel())
				{
					WallDoorToOpen->ToggleDoorOpenClose();
				}
				break;
			}
			case RewardTypes::REWARD_ITEMLOOT:
			{
				const TSubclassOf<ALootableItemBase> LootableItem = LootableRewardMap.FindRef(Key);
				if (LootableItem->IsValidLowLevel())
				{
					FVector Location = GetActorLocation();
					Location.Z += 200.0f;

					FRotator Rotation = FRotator::ZeroRotator;

					GetWorld()->SpawnActor<ALootableItemBase>(LootableItem->GetClass(), Location, Rotation);

				}
				break;
			}
			case RewardTypes::REWARD_RECALLSTATION:
				break;
			default:
				break;
			}

			
		}
	}

}

void ALevelChunkReward::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ALevelChunkReward::PostInitializeComponents()
{
	Super::PostInitializeComponents();
	
	ParticleComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	ParticleComponent->SetTemplate(TemplateParticle);

	SoundComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	SoundComponent->SetSound(SoundEffectToPlay);

	MusicComponent->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetIncludingScale);
	MusicComponent->SetSound(MusicToPlay);
}

void ALevelChunkReward::OnConstruction(const FTransform& Transform)
{

}

