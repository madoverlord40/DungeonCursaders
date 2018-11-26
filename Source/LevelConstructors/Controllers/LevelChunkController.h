/*!
 * \file LevelChunkController.h
 *
 * \author James Hanley
 * \date May 2018
 */
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "LevelChunkController.generated.h"

UENUM(BlueprintType)
enum class LevelClearRequiementsType : uint8
{
	/* NO requiements, if any doors, they will be open automatically*/
	REQUIREMENT_NONE,
	/* all spawned enemies must be killed*/
	REQUIREMENT_KILLALL,
	/* Killl a speciific number of enemies, see LevelChunkController::EnemyKillsNeeded*/
	REQUIREMENT_KILLENEMIES,
	/* key item, could be dropped from enemy, or puzzle completion*/
	REQUIREMENT_KEYITEM,
	/* must complete puzzle, might reward a key item to proceed, or just completion*/
	REQUIREMENT_PUZZLE

};
/*!
 * \class ALevelChunkController
 *
 *\ Controls a grid formation of LevelConstructors for the purpose of setting up areas in the game
 *\  level that has enemies, traps, and puzzles. Spawning one of these will make it easier to set up
 *\  the game level. In a blueprint, assign level constructor classes and they will be created
 *\  inside the blueprint viewport for moving around and aligning and setting properties.
*
 * \author James Hanley
 * \date May 2018
 */
UCLASS()
class DUNGEONCRUSADES_API ALevelChunkController : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALevelChunkController();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void PostInitProperties() override;

	
	/** * Called when an actor is done spawning into the world (from UWorld::SpawnActor).
	* For actors with a root component, the location and rotation will have already been set.
	* Takes place after any construction scripts have been called 	*/
	virtual void PostActorCreated() override;

	/** Allow actors to initialize themselves on the C++ side */
	virtual void PostInitializeComponents() override;

	/** Called after all the components in the Components array are registered */
	virtual void PostRegisterAllComponents() override;

	/*** Called when an instance of this class is placed (in editor) or spawned.
	* @param	Transform			The transform the actor was constructed at. 	*/
	virtual void OnConstruction(const FTransform& Transform) override;

	//************************************
	// Method:    public  OnNotifyEnemyDeath
	// Returns:   void
	// Purpose: Reports back that one of the enemies that was spawned, has been killed.
	//		Then we can update the spawned enemy array, and check to see if room completion requirements have been met.
	// Parameter: const FName EnemyObjectName --->
	//************************************
	void OnNotifyEnemyDeath(const FName EnemyObjectName);


	//************************************
	// Method:    public  OnNotifyPuzzleCompleted
	// Returns:   void
	// Purpose:  Marks a puzzle object complete and checks to see if all puzzles required to leave area are completed.
	// Parameter: const FName & PuzzleObjectName --->The name of the puzzle object in the array of puzzle objects to mark complete
	//************************************
	void OnNotifyPuzzleCompleted(const FName& PuzzleObjectName);

	bool CheckRequirementsCompleted();

	UFUNCTION()
	virtual void OnBeginBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);

	UFUNCTION()
	virtual void OnEndBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ResetRoom();

protected:

	void SpawnRewards();
	
	UPROPERTY(Transient, VisibleAnywhere, BlueprintReadWrite, Category = "Components")
	class AWallDoorwayConstructor* DoorwayToOpen;
	
	/**  * Trigger box used to detect player entering or leaving the controllers area 	 */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	class UBoxComponent* BoxTriggerComponent;
	
	/** Map of Enemy Classes to spawn in this level chunk controller area */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Configuration")
	TMap<FName, TSubclassOf<class AEnemyCharacterBase>> EnemyClassesToSpawn;

	/** Map of spawn transforms to use when spawning the enemies in the controller area
	 *   Map names must mach with EnemyClassesToSpawn * */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Enemy Configuration")
	TMap<FName, FTransform> EnemySpawnTransforms;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Box Extents")
	FVector BoxExtents;

	/** Map of actually spawned enemy pointers, names will match the classes to spawn */
	UPROPERTY(Transient, SaveGame)
	TMap<FName, class AEnemyCharacterBase*> SpawnedEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clear Requiements")
	TArray<LevelClearRequiementsType> ClearRequirements;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Clear Requiements")
	uint8 EnemyKillsNeeded;

	uint8 EnemyKillCounter;
	uint32 bHasRoomBeenCleared : 1;
};
