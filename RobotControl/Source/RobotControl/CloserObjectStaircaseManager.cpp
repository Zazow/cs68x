// Fill out your copyright notice in the Description page of Project Settings.


#include "CloserObjectStaircaseManager.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"

// Sets default values
ACloserObjectStaircaseManager::ACloserObjectStaircaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));
	LeftMeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Left Mesh Holder"));
	RightMeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Right Mesh Holder"));

	LeftMeshHolder->SetupAttachment(RootComponent);
	RightMeshHolder->SetupAttachment(RootComponent);

	LeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Mesh"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Mesh"));

	LeftMesh->SetupAttachment(LeftMeshHolder);
	RightMesh->SetupAttachment(RightMeshHolder);
}

#if WITH_EDITOR
void ACloserObjectStaircaseManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACloserObjectStaircaseManager, DistanceBetweenObjects)) {
		LeftMeshHolder->SetRelativeLocation(FVector(0, DistanceBetweenObjects / 2.f, 0));
		RightMeshHolder->SetRelativeLocation(FVector(0, -DistanceBetweenObjects / 2.f, 0));
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACloserObjectStaircaseManager, MeshToUse)) {
		LeftMesh->SetStaticMesh(MeshToUse);
		RightMesh->SetStaticMesh(MeshToUse);
	}
}
#endif


void ACloserObjectStaircaseManager::PositionMeshes(bool bWasResponseCorrect, bool bWasItCleanserTrial) {

	int Rand = FMath::RandBool() ? 1 : -1;
	CloserMesh = (Rand + 1)/2;

	if (!bWasItCleanserTrial) {


		CurrentDistance += bWasResponseCorrect ? -1*StreakMultiplier : 2*StreakMultiplier;
		CurrentDistance = FMath::Clamp(CurrentDistance, 1.f, CurrentDistance);
	
		StreakMultiplier = (bLastResponse == bWasResponseCorrect) ? StreakMultiplier + 1 : 1;

		bLastResponse = bWasResponseCorrect;
		
	}

	LeftMesh->SetRelativeLocation(FVector(-Rand * CurrentDistance / 2.f, 0.f, 0.f));
	RightMesh->SetRelativeLocation(FVector(Rand * CurrentDistance / 2.f, 0.f, 0.f));

	LeftMesh->SetRelativeRotation(FMath::VRand().Rotation());
	RightMesh->SetRelativeRotation(FMath::VRand().Rotation());

	if (bIsCleanserTrial) {
		auto ActorLocation = GetActorLocation();
		ActorLocation.X = ((FMath::FRand() - 0.5f) * 2 * MaxDistanceFromOrigin) + XOrigin;
		SetActorLocation(ActorLocation);
	}
}

void ACloserObjectStaircaseManager::Response(int CloserMeshResponse) {

	auto CurrentTime = FDateTime::UtcNow();
	if (!bIsCleanserTrial) {
		
		float Time = (CurrentTime - LastResponseTimeStamp).GetTotalSeconds();
		


		History->AddRow(FName(EName::NAME_None, TrialNumber), FCloserObjectHistoryPoint(
			CurrentDistance,
			Time,
			CloserMeshResponse == CloserMesh
		));
		TrialNumber++;
	}

	auto bPreviousTrial = bIsCleanserTrial;
	//randomly decide whether this is a cleanser trial or not: 40% of the time.
	bIsCleanserTrial = FMath::FRand() < 0;
	LastResponseTimeStamp = CurrentTime;
	PositionMeshes(CloserMeshResponse == CloserMesh, bPreviousTrial);
}

// Called when the game starts or when spawned
void ACloserObjectStaircaseManager::BeginPlay()
{
	Super::BeginPlay();
	LastResponseTimeStamp = FDateTime::UtcNow();

	XOrigin = GetActorLocation().X;

	PositionMeshes(false, false);

	//History = NewObject<UDataTable>(this, FName(TEXT("Closer Object History Table")));
	//History->StaticRegisterNativesUObject();
	//History->RowStruct = FCloserObjectHistoryPoint::StaticStruct();

	History->EmptyTable();
}

void ACloserObjectStaircaseManager::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
#if WITH_EDITOR
	if (History)
	{
		auto CSV = History->GetTableAsCSV(EDataTableExportFlags::UsePrettyPropertyNames);
		auto Path = FPaths::GameLogDir().Append(FString(TEXT("Staircase.csv")));
		FFileHelper::SaveStringToFile(CSV, *Path);
	}
#endif
}

// Called every frame
void ACloserObjectStaircaseManager::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

