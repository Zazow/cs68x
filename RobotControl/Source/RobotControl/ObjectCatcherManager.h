// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "VR_Pawn.h"
#include "ObjectCatcherManager.generated.h"


USTRUCT()
struct FObjectCatcherHistoryPoint : public FTableRowBase {
	GENERATED_BODY()

	FObjectCatcherHistoryPoint() : FTableRowBase() {

	}

	FObjectCatcherHistoryPoint(FVector CPos, FQuat CQuat,FVector BPos, float Time, int BNum, float Score, float Dist) : FTableRowBase() {
		ControllerPosition = CPos;
		ControllerRotation = CQuat;
		BallPosition = BPos;
		this->Time = Time;
		BallNumber = BNum;
		this->Score = Score;
		XYDistance = Dist;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector ControllerPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FQuat ControllerRotation;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FVector BallPosition;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int BallNumber;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Time;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Score;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float XYDistance;
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

	int RowNumber;
	float TimeSinceStart;

	float RoundScore = 0;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
