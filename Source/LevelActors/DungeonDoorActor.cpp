// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonDoorActor.h"
#include "Runtime/Engine/Classes/Components/StaticMeshComponent.h"
#include "Runtime/Engine/Classes/Engine/StaticMesh.h"
#include "Runtime/Engine/Classes/Components/BoxComponent.h"

// Sets default values
ADungeonDoorActor::ADungeonDoorActor(const FObjectInitializer& Initializer) : Super(Initializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	RootSceneComp = Initializer.CreateDefaultSubobject<USceneComponent>(this, TEXT("RootSceneComp"));
	RootSceneComp->SetupAttachment(RootComponent);

	DoorMeshComp = Initializer.CreateDefaultSubobject<UStaticMeshComponent>(this, TEXT("DoorMeshComp"));
	DoorMeshComp->SetupAttachment(RootSceneComp);

	DoorInteractBox = Initializer.CreateDefaultSubobject<UBoxComponent>(this, TEXT("Door Interact"));
	DoorInteractBox->SetupAttachment(DoorMeshComp);
}

// Called when the game starts or when spawned
void ADungeonDoorActor::BeginPlay()
{
	Super::BeginPlay();
	SetDoorActive(true);
}

// Called every frame
void ADungeonDoorActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DoorIsActive && DoorIsChanging)
	{
		const float RotateTo = IsDoorOpen ? DoorOpenAngle : DoorClosedAngle;

		FRotator Rotation = DoorMeshComp->GetRelativeTransform().GetRotation().Rotator();
		DoorRotationAngle = Rotation.Yaw;
		DoorRotationAngle = FMath::FInterpConstantTo(DoorRotationAngle, RotateTo, DeltaTime, 10.0f);

		Rotation.Yaw = DoorRotationAngle;
		DoorMeshComp->SetRelativeRotation(Rotation);

		if (FMath::IsNearlyEqual(DoorRotationAngle, RotateTo))
		{
			DoorIsChanging = false;
		}
	}
}

void ADungeonDoorActor::PostActorCreated()
{
	Super::PostActorCreated();

	if (IsValid(DoorMesh) && IsValid(DoorMeshComp))
	{
		DoorMeshComp->AttachToComponent(RootSceneComp, FAttachmentTransformRules::SnapToTargetIncludingScale);
		DoorMeshComp->SetStaticMesh(DoorMesh);

		DoorInteractBox->AttachToComponent(RootSceneComp, FAttachmentTransformRules::KeepRelativeTransform);

		if (RandomizeKey && RandomKeyTypes.Num() > 0)
		{
			const int random = FMath::RandRange(0, RandomKeyTypes.Num());

			KeyTypeToOpenDoor = RandomKeyTypes[random];
		}
	}
}

void ADungeonDoorActor::SetDoorCanOpen(bool CanOpen)
{
	DoorCanBeOpened = CanOpen;
}

void ADungeonDoorActor::PerformDoorOpenClose(bool OpenDoor)
{
	if (DoorCanBeOpened && DoorIsActive)
	{
		IsDoorOpen = OpenDoor;
		DoorIsChanging = true;

		if (IsDoorOpen)
		{
			DoorInteractBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
		else
		{
			DoorInteractBox->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		}
	}
}

void ADungeonDoorActor::SetDoorActive(bool Active)
{
	DoorIsActive = Active;
	DoorMeshComp->SetVisibility(Active);
}

void ADungeonDoorActor::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	
}

const FString ADungeonDoorActor::GetDoorDetails() const
{
	FString Text = DoorDetails;

	switch (KeyTypeToOpenDoor)
	{
	case DoorKeyTypes::BRONZE_KEY:
		Text.Append("\nYou need a Bronze Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::BRASS_KEY:
		Text.Append("\nYou need a Brass Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::IRON_KEY:
		Text.Append("\nYou need a Iron Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::SILVER_KEY:
		Text.Append("\nYou need a Silver Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::GOLD_KEY:
		Text.Append("\nYou need a Gold Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::SKELETON_KEY:
		Text.Append("\nYou need a Skeleton Key to open this door. If you have the correct key, click Unlock button!");
		break;
	case DoorKeyTypes::NONE_KEY:
		Text.Append("\nYou do not need a key to open this door, it is already unlocked!");
		break;
	default:
		break;
	}

	return Text;
}

bool ADungeonDoorActor::CanDoorBeOpened() const
{
	return (DoorCanBeOpened && DoorIsActive);
}

void ADungeonDoorActor::SetEnableHighlight(bool enable)
{
	DoorMeshComp->SetRenderCustomDepth(enable);
}

