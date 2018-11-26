// Fill out your copyright notice in the Description page of Project Settings.

#pragma once


#include "GameFramework/Actor.h"
#include "Characters/DataStructures/DungeonDoorKey.h"
#include "DungeonDoorActor.generated.h"

class UBoxComponent;
class UStaticMeshComponent;

UCLASS()
class DUNGEONCRUSADES_API ADungeonDoorActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonDoorActor(const FObjectInitializer& Initializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void PostActorCreated() override;

	void SetDoorCanOpen(bool CanOpen);

	void PerformDoorOpenClose(bool OpenDoor);

	void SetDoorActive(bool Active);

	virtual void PostInitializeComponents() override;

	DoorKeyTypes GetKeyTypeToOpenDoor() const { return KeyTypeToOpenDoor; }

	const FName& GetDoorName() const { return DoorName; }

	const FString GetDoorDetails() const;

	bool CanDoorBeOpened() const;

	void SetEnableHighlight(bool enable);

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USceneComponent* RootSceneComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* DoorMeshComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UBoxComponent* DoorInteractBox;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UStaticMesh* DoorMesh;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DoorOpenAngle;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float DoorClosedAngle;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsDoorOpen;

	//can this door be opened, controlled from level controller
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 DoorCanBeOpened : 1;
	//sets the door to active/inactive
	//inactive means its not visible and wont respond to open/close
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 DoorIsActive : 1;

	//can we open the door from both directions? if not then
	//back trigger will always do nothing
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 DoorIsBiDirectional : 1;

	//used for interpolating the door rotation
	float DoorRotationAngle;

	//flaged for the tick to open/close interp
	uint32 DoorIsChanging : 1;

	//name of the door, not the key map name
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DoorName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FString DoorDetails;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName DoorKeyName;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	DoorKeyTypes KeyTypeToOpenDoor;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	uint32 RandomizeKey : 1;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<DoorKeyTypes> RandomKeyTypes;
};
