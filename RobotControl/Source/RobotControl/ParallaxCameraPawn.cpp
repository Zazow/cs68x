// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallaxCameraPawn.h"
#include "GameFramework/PlayerController.h"
#include "EngineUtils.h"


//THIRD_PARTY_INCLUDES_START
//#include <NloptLibLibrary/nlopt.h>
//THIRD_PARTY_INCLUDES_END

// Sets default values
AParallaxCameraPawn::AParallaxCameraPawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	AutoPossessPlayer = EAutoReceiveInput::Player0;
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	SetRootComponent(Camera);
}

float AParallaxCameraPawn::GetImageSpaceSumDistance()
{
	auto Controller = GetController<APlayerController>();
	if (!Controller) {
		UE_LOG(LogTemp, Error, TEXT("GetImageSpaceSumDistance failed because AParallaxCameraPawn is not being controlled by a player!"));
		return -1.f;
	}

	//nlopt::opt(nlopt::algorithm::AUGLAG, 2);
	// test NLopt:
	//UE_LOG(LogTemp, Error, TEXT("%s"), nlopt_algorithm_name(nlopt_algorithm::NLOPT_AUGLAG));
	
	//ExampleLibraryFunction();

	TArray<TPair<AStaticMeshActor*, FVector2D>> ScreenVisibleObjects;

	// loop through all meshes in world:
	for (TObjectIterator<AStaticMeshActor> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		AStaticMeshActor* Actor = *Itr;
		FVector2D ScreenPosition;
		bool Visible = Controller->ProjectWorldLocationToScreen(Actor->GetActorLocation(), ScreenPosition);
		if (Visible) {
			ScreenVisibleObjects.Add(TPair<AStaticMeshActor*, FVector2D>(Actor, ScreenPosition));
		}
	}

	//auto test = nlopt_create(nlopt_algorithm::NLOPT_GD_STOGO, 0);

	float Sum = 0.f;

	//calculate the distances:
	for (int i = 0; i < ScreenVisibleObjects.Num(); i++) {
		for (int j = i; j < ScreenVisibleObjects.Num(); j++) {
			Sum += FVector2D::Distance(ScreenVisibleObjects[i].Value, ScreenVisibleObjects[j].Value);
		}
	}

	return Sum;
}

// Called when the game starts or when spawned
void AParallaxCameraPawn::BeginPlay()
{
	Super::BeginPlay();
	this->SetActorLocation(InterpolationPoints[0]);
	this->SetActorRotation((CenterPoint - InterpolationPoints[0]).Rotation());
}

// Called every frame
void AParallaxCameraPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	auto Location = GetActorLocation();
	if (Location.Equals(InterpolationPoints[InterpolationIndex], 1)) {
		InterpolationIndex = (InterpolationIndex + 1 == InterpolationPoints.Num()) ? InterpolationIndex : (InterpolationIndex + 1);
	}
	SetActorLocation(FMath::VInterpConstantTo(Location, InterpolationPoints[InterpolationIndex], DeltaTime, InterpSpeed));
	SetActorRotation((CenterPoint - Location).Rotation());
}

// Called to bind functionality to input
void AParallaxCameraPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

