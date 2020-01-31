// Fill out your copyright notice in the Description page of Project Settings.


#include "ObjectCatcherManager.h"
#include "Serialization/JsonWriter.h"
#include "BallToCatch.h"
#include "Misc/FileHelper.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Kismet/KismetSystemLibrary.h"


// Sets default values
AObjectCatcherManager::AObjectCatcherManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	GroundPlane = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Ground Plane"));
	SetRootComponent(GroundPlane);

	Cylinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CylinderMesh"));
	Cylinder->SetupAttachment(RootComponent);

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
	//Transform.SetScale3D((FVector)0.000001);

	auto Ball = GetWorld()->SpawnActorDeferred<ABallToCatch>(BallClass, Transform, this);
	if (Ball) {
		Ball->ManagerObject = this;
		Ball->FinishSpawning(Transform);
		Ball->Gravity = BallGravity;
		CurrentBallNumber++;
		CurrentFallingBall = Ball;
	}
}

float AObjectCatcherManager::GetCatcherToBallDistance_Implementation()
{
	auto XYBallPos = CurrentFallingBall->GetActorLocation();
	XYBallPos.Z = 0;
	auto CylinderXYPos = Cylinder->GetComponentLocation();
	CylinderXYPos.Z = 0;

	return FVector::Distance(XYBallPos, CylinderXYPos);
}

void AObjectCatcherManager::OnBallFell()
{
	if (CurrentBallNumber == NumberOfBallsToDrop) {
		// Increment ball number just in case the quit game request is queued. This will let us know that the ball fell or got caught in the log file
		CurrentBallNumber++;
		UKismetSystemLibrary::QuitGame(GetWorld(), Cast<APlayerController>(PlayerPawn->GetController()), EQuitPreference::Quit, true);
	}
	else {
		SpawnBall();
		RoundScore = 0;
	}
}

void AObjectCatcherManager::OnBallCaught()
{
	CaughtBalls++;
	if (CurrentBallNumber == NumberOfBallsToDrop) {
		// Increment ball number just in case the quit game request is queued. This will let us know that the ball fell or got caught in the log file
		CurrentBallNumber++;
		UKismetSystemLibrary::QuitGame(GetWorld(), Cast<APlayerController>(PlayerPawn->GetController()), EQuitPreference::Quit, true);
	}
	else {
		SpawnBall();
		RoundScore = 0;
	}

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

		auto FileName = LogFileName.Append(MotionNames[PlayerPawn->MotionType].Append(FString(TEXT(".csv"))));
		auto Path = FPaths::GameLogDir().Append(FileName);

		FFileHelper::SaveStringToFile(CSV, *Path);
	}
#endif
}

// Called every frame
void AObjectCatcherManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!bIsBallPreping) {

		auto DistanceNormalized = GetCatcherToBallDistance() * DeltaTime;
	
		RoundScore += DistanceNormalized;
		TotalScore += DistanceNormalized;

		History->AddRow(FName(EName::NAME_None, RowNumber), FObjectCatcherHistoryPoint(
			Cylinder->GetComponentLocation(),
			CurrentFallingBall->GetActorLocation(),
			DeltaTime,
			CurrentBallNumber,
			RoundScore,
			TotalScore,
			DistanceNormalized)
		);
		RowNumber++;
	}
	else {
		// prep the ball:
		CurrentPrepTime += DeltaTime;
		//auto Scale = (CurrentPrepTime / BallPrepTime) * ((FVector)0.1);
		CurrentFallingBall->MaterialInstance->SetScalarParameterValue(FName(TEXT("Opacity")), CurrentPrepTime / BallPrepTime);
		//CurrentFallingBall->SetActorScale3D(Scale);
		if (CurrentPrepTime >= BallPrepTime) {
			CurrentPrepTime = 0.f;
			CurrentFallingBall->StaticMesh->SetSimulatePhysics(true);
			bIsBallPreping = false;
		}
	}
	TimeSinceStart += DeltaTime;
}

