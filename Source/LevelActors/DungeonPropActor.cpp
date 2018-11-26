// Fill out your copyright notice in the Description page of Project Settings.

#include "DungeonPropActor.h"


// Sets default values
ADungeonPropActor::ADungeonPropActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ADungeonPropActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ADungeonPropActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

