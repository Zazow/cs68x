// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectCatcherManager.h"
#include "Serialization/JsonWriter.h"
#include "BallToCatch.h"
#include "Misc/FileHelper.h"


// Sets default values
AObjectCatcherManager::AObjectCatcherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GroundPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Plane"));
	SetRootComponent(GroundPlane);
	

	SpawnBox = CreateDefaultSubobject<UBoxComponent>(TEXT("Spawn Box"));
	SpawnBox->SetupAttachment(GroundPlane);
	SpawnBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);


	ControllerStartingPosition = CreateDefaultSubobject<USceneComponent>(TEXT("Controller Starting Position"));
	ControllerStartingPosition->SetupAttachment(RootComponent);

}

void AObjectCatcherManager::SpawnBall()
{
	bIsBallPreping = true;
	auto Box = SpawnBox->Bounds.GetBox();
	auto SpawnPoint = FMath::RandPointInBox(Box);

	auto Transform = FTransform(SpawnPoint);
	Transform.SetScale3D((FVector)0.000001);

	auto Ball = GetWorld()->SpawnActorDeferred<ABallToCatch>(BallClass, Transform, this);
	if (Ball) {
		Ball->ManagerObject = this;
		Ball->FinishSpawning(Transform);
		Ball->Gravity = BallGravity;
		CurrentBallNumber++;
		CurrentFallingBall = Ball;
	}
}

void AObjectCatcherManager::OnBallFell()
{
	SpawnBall();
	RoundScore = 0;
}

void AObjectCatcherManager::OnBallCaught()
{
	CaughtBalls++;
	SpawnBall();
	RoundScore = 0;
}

// Called when the game starts or when spawned
void AObjectCatcherManager::BeginPlay()
{
	Super::BeginPlay();
	//History = NewObject<UDataTable>(this);
	History->EmptyTable();
	//History->RowStruct = FObjectCatcherHistoryPoint::StaticStruct();
	SpawnBall();

}

void AObjectCatcherManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	// Export data table to json:
#if WITH_EDITOR
	if (History)
	{
		auto CSV = History->GetTableAsCSV(EDataTableExportFlags::UsePrettyPropertyNames);

		auto Path = FPaths::GameLogDir().Append(FString(TEXT("TEST_OUT.csv")));

		FFileHelper::SaveStringToFile(CSV, *Path);
	}
#endif
}

// Called every frame
void AObjectCatcherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsBallPreping) {
		auto XYBallPos = CurrentFallingBall->GetActorLocation();
		XYBallPos.Z = 0;
		auto XYControllerPos = PlayerPawn->RightController->GetComponentLocation();
		XYControllerPos.Z = 0;

		auto Distance = FVector::Distance(XYBallPos, XYControllerPos);

		RoundScore += Distance * DeltaTime;

		History->AddRow(FName(EName::NAME_None, RowNumber), FObjectCatcherHistoryPoint(PlayerPawn->RightController->GetComponentLocation(),
			PlayerPawn->RightController->GetComponentQuat(),
			CurrentFallingBall->GetActorLocation(),
			TimeSinceStart,
			CurrentBallNumber,
			RoundScore,
			Distance)
		);
		RowNumber++;
	}
	else {
		// prep the ball:
		CurrentPrepTime += DeltaTime;
		auto Scale = (CurrentPrepTime / BallPrepTime) * ((FVector)0.1);
		CurrentFallingBall->SetActorScale3D(Scale);
		if (CurrentPrepTime >= BallPrepTime) {
			CurrentPrepTime = 0.f;
			CurrentFallingBall->StaticMesh->SetSimulatePhysics(true);
			bIsBallPreping = false;
		}
	}
	TimeSinceStart += DeltaTime;
}

