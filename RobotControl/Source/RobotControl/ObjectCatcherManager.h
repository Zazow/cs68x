// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "VR_Pawn.h"
#include "ObjectCatcherManager.generated.h"


USTRUCT(BlueprintType)
struct FObjectCatcherHistoryPoint : public FTableRowBase {
	GENERATED_BODY()

	FObjectCatcherHistoryPoint() : FTableRowBase() {

	}

	FObjectCatcherHistoryPoint(FVector CPos,FVector BPos, float Time, int BNum, float RoundScore, float TotalScore, float Dist) : FTableRowBase() {
		ControllerPosition = CPos;
		BallPosition = BPos;
		this->DeltaTime = Time;
		BallNumber = BNum;
		this->RoundScore = RoundScore;
		this->TotalScore = TotalScore;
		XYDistanceNormalized = Dist;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControllerPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BallPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BallNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DeltaTime;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RoundScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TotalScore;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float XYDistanceNormalized;
};



UCLASS()
class ROBOTCONTROL_API AObjectCatcherManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjectCatcherManager();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* GroundPlane;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Cylinder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UBoxComponent* SpawnBox;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSubclassOf<class ABallToCatch> BallClass;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	ABallToCatch* CurrentFallingBall;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CurrentBallNumber;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int CaughtBalls;

	UFUNCTION(BlueprintCallable)
	void SpawnBall();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	float GetCatcherToBallDistance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float BallPrepTime = 1;
	float CurrentPrepTime;
	bool bIsBallPreping = true;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* ControllerStartingPosition;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector BallGravity;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AVR_Pawn* PlayerPawn;

	UFUNCTION(BlueprintCallable)
	void OnBallFell();

	UFUNCTION(BlueprintCallable)
	void OnBallCaught();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* History;

	TMap<EMotionType, FString> MotionNames = {
		TPair<EMotionType, FString>(EMotionType::MT_NONE, FString(TEXT("_No_Motion"))),
		TPair<EMotionType, FString>(EMotionType::MT_SWEEPING, FString(TEXT("_Sweeping_Motion"))),
		TPair<EMotionType, FString>(EMotionType::MT_SWEEPING_LOOKAT, FString(TEXT("_Sweeping_Motion_Lookat"))),
		TPair<EMotionType, FString>(EMotionType::MT_REVOLVING_LOOKAT, FString(TEXT("_Revolving_Motion_Lookat"))),
		TPair<EMotionType, FString>(EMotionType::MT_LOOKING, FString(TEXT("_Just_Looking_Motion")))
	};

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString LogFileName;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	int NumberOfBallsToDrop = -1;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Settings")
	float MaxSizeDelta = 0.f;

	int RowNumber;
	float TimeSinceStart;

	float RoundScore = 0;
	float TotalScore = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
