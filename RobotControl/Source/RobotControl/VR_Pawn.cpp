// Fill out your copyright notice in the Description page of Project Settings.


#include "VR_Pawn.h"
#include "MyGameInstance.h"

// Sets default values
AVR_Pawn::AVR_Pawn()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("VR_Origin"));
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));

	HandsOrigin = CreateDefaultSubobject<USceneComponent>(TEXT("Hadnds Origin"));
	HandsOrigin->SetupAttachment(Camera);

	RightController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Right Controller"));
	RightController->SetupAttachment(HandsOrigin);
	//RightController->MotionSource = FName(TEXT("Right"));
	RightControllerMouseInteraction = CreateDefaultSubobject<UMouseInteractionComponent>(TEXT("Right Controller Mouse Interaction"));
	RightControllerMouseInteraction->SetupAttachment(RootComponent);

	LeftController = CreateDefaultSubobject<UMotionControllerComponent>(TEXT("Left Controller"));
	LeftController->SetupAttachment(HandsOrigin);
	//LeftController->MotionSource = FName(TEXT("Left"));
}

void AVR_Pawn::SimpleMotion(float DeltaTime)
{
	alpha += Direction * SimpleParallaxSpeed * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("MOCING"));
	float Y = FMath::InterpEaseInOut<float>(-Distance, Distance, alpha, 2);
	Camera->SetRelativeLocation(FVector(0.f, Y, 0.f));
	
	
	if (alpha >= 1 || alpha <= 0) {
		alpha = alpha >= 1 ? 1 : 0;
		Direction *= -1;
	}
}

void AVR_Pawn::SimpleMotionLookAt(float DeltaTime)
{
	SimpleMotion(DeltaTime);
	auto LookAtVec = -Camera->GetComponentLocation();
	Camera->SetWorldRotation(LookAtVec.Rotation());
}

void AVR_Pawn::SphericalMotion(float DeltaTime)
{
	alpha += Direction * SimpleParallaxSpeed * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("MOVING"));
	auto u = 2.f * alpha - 1.f;
	Camera->SetWorldLocation(FVector(-RadiusSqrt*FMath::Cos(u * Angle/180.f*PI), RadiusSqrt*FMath::Sin(u * Angle / 180.f * PI), 85.f));

	if (alpha >= 1 || alpha <= 0) {
		alpha = alpha >= 1 ? 1 : 0;
		Direction *= -1;
	}

	auto LookAtVec = -Camera->GetComponentLocation();
	Camera->SetWorldRotation(LookAtVec.Rotation());
}

void AVR_Pawn::SimpleRotattion(float DeltaTime)
{
	alpha += Direction * SimpleParallaxSpeed * DeltaTime;
	//UE_LOG(LogTemp, Warning, TEXT("MOCING"));
	float Yaw = FMath::InterpEaseInOut<float>(-15, 15, alpha, 2);
	SetActorRotation(FRotator(-20.f, Yaw, 0.f));

	if (alpha >= 1 || alpha <= 0) {
		alpha = alpha >= 1 ? 1 : 0;
		Direction *= -1;
	}
}

// Called when the game starts or when spawned
void AVR_Pawn::BeginPlay()
{
	Super::BeginPlay();
	
	auto CameraOption = UMyGameInstance::CameraMovement;

	if (bUseGameSettings) {
		if (CameraOption == ECameraMovement::CM_LOCKED) {
			Camera->bLockToHmd = true;
		}
		else {
			CameraMotionUpdater = &AVR_Pawn::SimpleMotionLookAt;
		}
	}
	else {
		switch (MotionType)
		{
		case EMotionType::MT_NONE:
			CameraMotionUpdater = nullptr;
			break;
		case EMotionType::MT_SWEEPING:
			CameraMotionUpdater = &AVR_Pawn::SimpleMotion;
			break;
		case EMotionType::MT_SWEEPING_LOOKAT:
			CameraMotionUpdater = &AVR_Pawn::SimpleMotionLookAt;
			break;
		case EMotionType::MT_REVOLVING_LOOKAT:
			CameraMotionUpdater = &AVR_Pawn::SphericalMotion;
			break;
		case EMotionType::MT_LOOKING:
			CameraMotionUpdater = &AVR_Pawn::SimpleRotattion;
			break;
		default:
			CameraMotionUpdater = nullptr;
			break;
		}
	}
}

// Called every frame
void AVR_Pawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// update whatever function we used for parallax
	if (CameraMotionUpdater) {
		(this->*CameraMotionUpdater)(DeltaTime);
	}


	// Trace for objects near left hand:
	TArray<FHitResult> Results;
	auto Start = LeftController->GetComponentLocation();
	auto End = Start + (FVector)1;
	FCollisionObjectQueryParams Params;
	
	GetWorld()->SweepMultiByChannel(Results, Start, End, FQuat::Identity, ECollisionChannel::ECC_Visibility, FCollisionShape::MakeSphere(16.f));

	for (auto Result : Results) {
		auto Actor = Result.GetActor();
		auto GrabbableActor = Cast<IGrabbableActor>(Actor);
		if (GrabbableActor) {
			if (GrabbableActor != LeftHandGrabbable) {
				if (LeftHandGrabbable) {
					LeftHandGrabbable->Execute_OnControllerFar(Actor);
				}
				GrabbableActor->Execute_OnControllerClose(Actor);
				LeftHandGrabbable = GrabbableActor;
			}
		}
	}
}

// Called to bind functionality to input
void AVR_Pawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

