// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/StaticMesh.h"
#include "GameFramework/Actor.h"
#include "Engine/DataTable.h"
#include "CloserObjectStaircaseManager.generated.h"


USTRUCT()
struct FCloserObjectHistoryPoint : public FTableRowBase {
	GENERATED_BODY()

	FCloserObjectHistoryPoint() : FTableRowBase() {

	}

	FCloserObjectHistoryPoint(float Distance, float Time, bool Correctness, float std, float slope) : FTableRowBase() {
		DistanceBetweenObjects = Distance;
		TimeToAnswer = Time;
		Answer = Correctness;
		Last30std = std;
		SlopeSince30Trials = slope;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceBetweenObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Answer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToAnswer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float Last30std;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SlopeSince30Trials;
};

UCLASS()
class ROBOTCONTROL_API ACloserObjectStaircaseManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACloserObjectStaircaseManager();

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float DistanceBetweenObjects;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float MaxDistanceFromOrigin;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* MeshHolder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* MeshHolderAlt;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* LeftMeshHolder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RightMeshHolder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* LeftMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* RightMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* LeftMeshHolderAlt;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RightMeshHolderAlt;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* LeftMeshAlt;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* RightMeshAlt;

	void TickRotate(float DeltaTime);

	void(ACloserObjectStaircaseManager::* TickRotateFunction)(float DeltaTime);

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float TransitionSpeed;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRandomizeSize;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SizeDelta=0.1;

	int CurrentMeshes;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TrialNumber=0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StreakMultiplier = 1;
	int UnStreakCounter;
	bool bShouldSkipDynamicStepSize = false;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLastResponse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDistance = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* MeshToUse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* History;

	FDateTime LastResponseTimeStamp;

	bool bAlternateMeshesTrial;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif

	UFUNCTION(BlueprintCallable)
	void PositionMeshes(bool bWasResponseCorrect, bool bAlternateMeshes);

	float GetStdOfLast30Distances();

	TArray<float> Distances;
	UFUNCTION(BlueprintCallable)
	void Response(int bCloserMeshResponse);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	int CloserMesh;
	float XOrigin;
};
