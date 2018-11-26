// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Runtime/Engine/Classes/Components/TextRenderComponent.h"
#include "LootableItemBase.generated.h"

UENUM(BlueprintType)
enum class LootableItemTypes : uint8
{
	HealthReplenishDrop,
	ManaReplanishDrop,
	/** Rewards experience points */
    ExperienceDrop,
	/* Rewards a stat modify drop, could be any stat, and up or down*/
    StatsModifyDrop,
	/* Adds status effect, may be good or bad*/
	StatusEffectModifyDrop,
	/* Relieves one bad status effect from player*/
    CureStatusDrop,
	/* MONEY!!!*/
    CurrencyDrop,
	/* Elemental Trap, ice.fire..etc, can be explosive*/
	ElementalTrapDrop,
	/* Could be any attack property, such as adding poison damage, or just increasing damage*/
    WeaponUpgradeDrop,
	/* Add resistance to the player, such as resist poison*/
	ResistanceUpgradeDrop,
	/* drops one of 'ItemObject' class type*/
    ItemDrop,
    /* Nothing Drop, for adding weights to possibly have nothing drop*/
	ITEMDROP_NONE
};

UCLASS()
class DUNGEONCRUSADES_API ALootableItemBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ALootableItemBase(const FObjectInitializer& Intitializer = FObjectInitializer::Get());

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTextRenderComponent* TextRenderer;
	
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    LootableItemTypes LootType;
    
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    class USphereComponent* RootBodyCollider;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
    USphereComponent* PickupCollider;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UParticleSystem* ItemParticle;


	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

    float Timer = 0.0f;
    bool DoPickupTimer = true;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

    LootableItemTypes GetLootItemType() { return LootType; }

    UFUNCTION()
    void OnBeginOverlap(UPrimitiveComponent * OverlappedComponent, AActor * OtherActor, UPrimitiveComponent * OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult);
    
    virtual void PostInitializeComponents() override;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemName;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FName ItemDescription;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    UParticleSystem* PickupEffect;
    
    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float DamageValue;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float EffectRadius;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    FVector SpawnForce;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    float CanPickupCooldown = 3.5f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly)
    USoundBase* PickupSound;

private:

	TWeakObjectPtr<APawn> PlayerCharacter;
};
