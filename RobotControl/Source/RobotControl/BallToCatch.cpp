// Fill out your copyright notice in the Description page of Project Settings.


#include "BallToCatch.h"

// Sets default values
ABallToCatch::ABallToCatch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMesh->SetEnableGravity(false);
	StaticMesh->SetSimulatePhysics(true);
}

// Called when the game starts or when spawned
void ABallToCatch::BeginPlay()
{
	Super::BeginPlay();
	MaterialInstance = StaticMesh->CreateDynamicMaterialInstance(0, MaterialToUse);
	
}

// Called every frame
void ABallToCatch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	StaticMesh->SetPhysicsLinearVelocity(Gravity);
}

