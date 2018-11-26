// Fill out your copyright notice in the Description page of Project Settings.

#include "LootableItemBase.h"
#include "Runtime/Engine/Classes/Components/SphereComponent.h"
#include "Runtime/Engine/Classes/Particles/ParticleSystem.h"
#include "Runtime/Engine/Classes/Kismet/GameplayStatics.h"
#include "Characters/Players/PlayerCharacterBase.h"

// Sets default values
ALootableItemBase::ALootableItemBase(const FObjectInitializer& Initializer) : Super(Initializer)
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
    RootBodyCollider = Initializer.CreateDefaultSubobject<USphereComponent>(this, FName("Root Collider"));
    RootBodyCollider->SetupAttachment(RootComponent);

    PickupCollider = Initializer.CreateDefaultSubobject<USphereComponent>(this, FName("Pickup Collider"));
    PickupCollider->SetupAttachment(RootBodyCollider);
    PickupCollider->OnComponentBeginOverlap.AddDynamic(this, &ALootableItemBase::OnBeginOverlap);

	ItemMesh = Initializer.CreateDefaultSubobject<UStaticMeshComponent>(this, FName("Item Mesh"));
	ItemMesh->SetupAttachment(RootComponent);

	TextRenderer = Initializer.CreateDefaultSubobject<UTextRenderComponent>(this, FName("Text Render"));

}

// Called when the game starts or when spawned
void ALootableItemBase::BeginPlay()
{
	Super::BeginPlay();
    if (IsValid(ItemParticle)) {
        UGameplayStatics::SpawnEmitterAttached(ItemParticle, RootBodyCollider);
    }

    
}

// Called every frame
void ALootableItemBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
    if (DoPickupTimer) {
        if (Timer < CanPickupCooldown) {
            RootBodyCollider->AddForce(SpawnForce * DeltaTime);
            Timer += DeltaTime;
        }
        else {
            PickupCollider->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

            DoPickupTimer = false;
        }
    }

	if (PlayerCharacter.IsValid())
	{
		FVector PlayerLocation = PlayerCharacter->GetActorLocation();
		FVector ItemLocation = GetActorLocation();
		FVector Direction = PlayerLocation - ItemLocation;
		Direction.Normalize();

		FQuat Rotation = FRotationMatrix::MakeFromX(Direction).Rotator().Quaternion();

		ItemMesh->SetRelativeRotation(Rotation);
	}
	else
	{
		PlayerCharacter = GetWorld()->GetFirstPlayerController()->GetPawnOrSpectator();
	}
}

void ALootableItemBase::OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
    
}

void ALootableItemBase::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    RootBodyCollider->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
    RootBodyCollider->SetCollisionProfileName(FName("PhysicsActor"));
    RootBodyCollider->SetSimulatePhysics(true);
    //RootBodyCollider->SetLinearDamping(5.0f);
    //RootBodyCollider->SetAngularDamping(5.0f);
    //RootBodyCollider->SetMassOverrideInKg(NAME_None, 100);
    PickupCollider->SetCollisionProfileName(FName("ItemPickupProfile"));
	PickupCollider->SetGenerateOverlapEvents(true);
    PickupCollider->SetCollisionEnabled(ECollisionEnabled::NoCollision);
    PickupCollider->SetSphereRadius(150.0f);

	ItemMesh->AttachToComponent(RootComponent, FAttachmentTransformRules::SnapToTargetNotIncludingScale);
	ItemMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	TextRenderer->AttachToComponent(ItemMesh, FAttachmentTransformRules::KeepRelativeTransform);
	TextRenderer->SetText(FText::FromName(ItemName));
	TextRenderer->SetHorizontalAlignment(EHTA_Center);
}

