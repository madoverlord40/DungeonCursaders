#pragma once
#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../WallDoorwayConstructor.h"
#include "LevelChunkReward.generated.h"


UENUM(BlueprintType)
enum class RewardTypes : uint8
{
	REWARD_NONE,
	REWARD_OPENDOOR,
	REWARD_ITEMLOOT,
	REWARD_RECALLSTATION
};

class ALootableItemBase;
class UParticleSystemComponent;
class USoundCue;
class UAudioComponent;

UCLASS(Blueprintable)
class DUNGEONCRUSADES_API ALevelChunkReward : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ALevelChunkReward(const FObjectInitializer& Initializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostInitializeComponents() override;

	void OnConstruction(const FTransform& Transform) override;
	
	/* Pass in a pointer reference to the weak object pointer that caches waht door to open*/
	void SetWallDoorRef(AWallDoorwayConstructor* WallDoorRef) { WallDoorToOpen = WallDoorRef; }

protected:
	/* Particle component to play the template particle*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UParticleSystemComponent * ParticleComponent;

	/*Plays the SoundEffectToPlay*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* SoundComponent;

	/* Plays the MusicToPlay*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UAudioComponent* MusicComponent;

	/* Particle system to play when this is spawned*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Particles")
	UParticleSystem* TemplateParticle;

	/*Sound Cue for any sound effects to play when this is spawned*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Configuration")
	USoundCue* SoundEffectToPlay;

	/* Cue to play music, if any*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound Configuration")
	USoundCue* MusicToPlay;

	/*A reference to a wall doorway constructor that has a door in it. There are functions in that to open the doorway*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward Configuration")
	TWeakObjectPtr<AWallDoorwayConstructor> WallDoorToOpen;

	/* This map holds a list of rewards types to give at level clear
	 * We could have it give several lootables plus open a door!
	 * The Map keys(FString) must match the keys of lootable rewards
	 * */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward Configuration")
	TMap<FString, RewardTypes> TypesOfRewardMap;

	/*Map of lootable reward classes to spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Reward Configuration")
	TMap<FString, TSubclassOf<ALootableItemBase>> LootableRewardMap;
};