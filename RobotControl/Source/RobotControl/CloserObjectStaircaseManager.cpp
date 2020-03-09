// Fill out your copyright notice in the Description page of Project Settings.


#include "CloserObjectStaircaseManager.h"
#include "Misc/FileHelper.h"
#include "Misc/DateTime.h"
#include "Kismet/KismetSystemLibrary.h"

// Sets default values
ACloserObjectStaircaseManager::ACloserObjectStaircaseManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Scene"));

	MeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh Holder"));
	MeshHolderAlt = CreateDefaultSubobject<USceneComponent>(TEXT("Mesh Holder Alternate"));

	MeshHolder->SetupAttachment(RootComponent);
	MeshHolderAlt->SetupAttachment(RootComponent);

	LeftMeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Left Mesh Holder"));
	RightMeshHolder = CreateDefaultSubobject<USceneComponent>(TEXT("Right Mesh Holder"));

	LeftMeshHolder->SetupAttachment(MeshHolder);
	RightMeshHolder->SetupAttachment(MeshHolder);

	LeftMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Mesh"));
	RightMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Mesh"));

	LeftMesh->SetupAttachment(LeftMeshHolder);
	RightMesh->SetupAttachment(RightMeshHolder);

	LeftMeshHolderAlt = CreateDefaultSubobject<USceneComponent>(TEXT("Left Mesh Holder Alternate"));
	RightMeshHolderAlt = CreateDefaultSubobject<USceneComponent>(TEXT("Right Mesh Holder Alternate"));

	LeftMeshHolderAlt->SetupAttachment(MeshHolderAlt);
	RightMeshHolderAlt->SetupAttachment(MeshHolderAlt);

	LeftMeshAlt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Left Mesh Alternate"));
	RightMeshAlt = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Right Mesh Alternate"));

	LeftMeshAlt->SetupAttachment(LeftMeshHolderAlt);
	RightMeshAlt->SetupAttachment(RightMeshHolderAlt);
}

void ACloserObjectStaircaseManager::TickRotate(float DeltaTime)
{
	// Determine whether we are rotating to face alternate or main meshes.
	int Direction = bAlternateMeshesTrial ? -1 : 1;
	auto CurrentYaw = GetActorRotation().Yaw;
	float TargetYaw = 180.f - 90 * (1.f - Direction) / 2.f;
	if (FMath::IsNearlyEqual(CurrentYaw, TargetYaw, 1.f)) {
		// We reached our desired rotation and can stop.
		TickRotateFunction = nullptr;
		return;
	}

	// Rotate the actor:
	SetActorRotation(FRotator(0.f, FMath::FInterpTo(CurrentYaw, TargetYaw, DeltaTime, TransitionSpeed), 0.f));
}

#if WITH_EDITOR
void ACloserObjectStaircaseManager::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) {
	Super::PostEditChangeProperty(PropertyChangedEvent);
	FName PropertyName = (PropertyChangedEvent.Property != nullptr) ? PropertyChangedEvent.Property->GetFName() : NAME_None;
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACloserObjectStaircaseManager, DistanceBetweenObjects)) {
		LeftMeshHolder->SetRelativeLocation(FVector(0, DistanceBetweenObjects / 2.f, 0));
		RightMeshHolder->SetRelativeLocation(FVector(0, -DistanceBetweenObjects / 2.f, 0));
		LeftMeshHolderAlt->SetRelativeLocation(FVector(0, DistanceBetweenObjects / 2.f, 0));
		RightMeshHolderAlt->SetRelativeLocation(FVector(0, -DistanceBetweenObjects / 2.f, 0));
	}
	if (PropertyName == GET_MEMBER_NAME_CHECKED(ACloserObjectStaircaseManager, MeshToUse)) {
		LeftMesh->SetStaticMesh(MeshToUse);
		RightMesh->SetStaticMesh(MeshToUse);
		LeftMeshAlt->SetStaticMesh(MeshToUse);
		RightMeshAlt->SetStaticMesh(MeshToUse);
	}
}
#endif


void ACloserObjectStaircaseManager::PositionMeshes(bool bWasResponseCorrect, bool bAlternateMeshes) {

	int Rand = FMath::RandBool() ? 1 : -1;
	CloserMesh = (Rand + 1)/2;

	

	if (bLastResponse != bWasResponseCorrect) {
		UnStreakCounter++;
	}
	else {
		UnStreakCounter = 0;
	}
	if (UnStreakCounter > 3 || CurrentDistance < 35.f || bShouldSkipDynamicStepSize) {
		bShouldSkipDynamicStepSize = true;
		CurrentDistance += bWasResponseCorrect ? -1 : 2;
		CurrentDistance = FMath::Clamp(CurrentDistance, 1.f, CurrentDistance);
	}
	else {
		CurrentDistance += bWasResponseCorrect ? -1 * StreakMultiplier : 2 * StreakMultiplier;
		CurrentDistance = FMath::Clamp(CurrentDistance, 1.f, CurrentDistance);
		StreakMultiplier = FMath::Clamp((bLastResponse == bWasResponseCorrect) ? StreakMultiplier + 1 : 1, 1.f, 4.f);
	}
	bLastResponse = bWasResponseCorrect;

	
	if (bAlternateMeshes) {
		LeftMeshAlt->SetRelativeLocation(FVector(-Rand * CurrentDistance / 2.f, 0.f, 0.f));
		RightMeshAlt->SetRelativeLocation(FVector(Rand * CurrentDistance / 2.f, 0.f, 0.f));

		LeftMeshAlt->SetRelativeRotation(FMath::VRand().Rotation());
		RightMeshAlt->SetRelativeRotation(FMath::VRand().Rotation());

		if (bRandomizeSize) {
			LeftMeshAlt->SetRelativeScale3D((FVector)FMath::FRandRange(1.f - SizeDelta, 1 + SizeDelta));
			RightMeshAlt->SetRelativeScale3D((FVector)FMath::FRandRange(1.f - SizeDelta, 1 + SizeDelta));
		}
	}
	else {
		LeftMesh->SetRelativeLocation(FVector(-Rand * CurrentDistance / 2.f, 0.f, 0.f));
		RightMesh->SetRelativeLocation(FVector(Rand * CurrentDistance / 2.f, 0.f, 0.f));

		LeftMesh->SetRelativeRotation(FMath::VRand().Rotation());
		RightMesh->SetRelativeRotation(FMath::VRand().Rotation());

		if (bRandomizeSize) {
			LeftMesh->SetRelativeScale3D((FVector)FMath::FRandRange(1.f - SizeDelta, 1 + SizeDelta));
			RightMesh->SetRelativeScale3D((FVector)FMath::FRandRange(1.f - SizeDelta, 1 + SizeDelta));
		}
	}
}

float ACloserObjectStaircaseManager::GetStdOfLast30Distances()
{
	float Sum = 0.f, Mean, Var = 0.f;
	for (int i = Distances.Num() - 20; i < Distances.Num(); i++) {
		Sum += Distances[i];
	}

	Mean = Sum / 20.f;

	for (int i = Distances.Num() - 20; i < Distances.Num(); i++) {
		Var += FMath::Pow(Distances[i] - Mean, 2);
	}

	return FMath::Sqrt(Var / 20.f);
}

void ACloserObjectStaircaseManager::Response(int CloserMeshResponse) {

	// if we are still rotating, ignore the response.
	if (TickRotateFunction) {
		return;
	}

	// add the current distance to keep track of the past distances
	Distances.Add(CurrentDistance);
	TrialNumber++;

	// calculate the standard deviation of the last 20 trials, otherwise fill in a -1.
	auto std = -1.f;
	auto Slope = -1.f;
	if (TrialNumber > 30) {
		std = GetStdOfLast30Distances();
		Slope = Distances[Distances.Num() - 1] - Distances[Distances.Num() - 30];
	}
	
	auto CurrentTime = FDateTime::UtcNow();
	float Time = (CurrentTime - LastResponseTimeStamp).GetTotalSeconds();
	History->AddRow(FName(EName::NAME_None, TrialNumber), FCloserObjectHistoryPoint(
		CurrentDistance,
		Time,
		CloserMeshResponse == CloserMesh,
		std,
		Slope
	));

	if (std > 0.f && std < 2.f && Slope > -5.f && Slope < 5.f) {
		UKismetSystemLibrary::QuitGame(GetWorld(), GetWorld()->GetFirstPlayerController(), EQuitPreference::Quit, true);
	}

	LastResponseTimeStamp = CurrentTime;
	PositionMeshes(CloserMeshResponse == CloserMesh, !bAlternateMeshesTrial);
	bAlternateMeshesTrial = !bAlternateMeshesTrial;
	TickRotateFunction = &ACloserObjectStaircaseManager::TickRotate;
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
	if (TickRotateFunction) {
		(this->*TickRotateFunction)(DeltaTime);
	}
}

