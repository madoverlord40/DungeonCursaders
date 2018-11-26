/*!
 * \file ModifyStatsDrop.h
 *
 * \author James Hanley
 * \date May 2018
 *
 * 
 */
 
#pragma once
 
#include "CoreMinimal.h"
#include "LootableItemBase.h"
#include "../../DataStructures/CombatData.h"
#include "ModifyStatsDrop.generated.h"

UCLASS()
class DUNGEONCRUSADES_API AModifyStatsDrop : public ALootableItemBase
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AModifyStatsDrop(const FObjectInitializer& Intitializer = FObjectInitializer::Get());

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

 public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	//called shortly after the constructor, and before begin play
	virtual void PostInitProperties() override;

	//Called After PostInitProperties, and after any changes in editor.
	void OnConstruction(const FTransform& Transform) override;

	//called shortly after the constructor, and before begin play
	virtual void PostInitializeComponents() override;

	//************************************
	// Method:    const public  GetStatToModify
	// Returns:   const StatModifyTypes
	// Purpose: returns const type for the stat to be modified.
	//************************************
	//const StatModifyTypes GetStatToModify() const { return StatToModify; }

	//************************************
	// Method:    const public  GetInternalStatModifier
	// Returns:   const FInternalStats*
	// Purpose: Gets a const pointer to the internal stat modifier, containing the stats to be modified.
	//************************************
	const FInternalStats* GetInternalStatModifier() const { return &InternalStatModifier; }

	//************************************
	// Method:    const public  GetInternalStatModifier
	// Returns:   const FExternalStats*
	// Purpose: returns const pointer to external stats to modify
	//************************************
	const FExternalStats* GetExternalStatModifier() const { return &ExternalStatModifier; }

	const float GetDurationTime() const { return EffectDurationTime; }

	const FName& GetEffectName() const { return StatEffectName; }

protected:

// 	UPROPERTY(EditAnywhere, BlueprintReadOnly)
// 	StatModifyTypes StatToModify;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FInternalStats InternalStatModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FExternalStats ExternalStatModifier;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	float EffectDurationTime;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	FName StatEffectName;

};