// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "DungeonPropActor.generated.h"

class UCapsuleComponent;

UCLASS()
class DUNGEONCRUSADES_API ADungeonPropActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonPropActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UStaticMeshComponent* PropMeshComp;

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCapsuleComponent* PropCollider;

};
