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

	FCloserObjectHistoryPoint(float Distance, float Time, bool Correctness) : FTableRowBase() {
		DistanceBetweenObjects = Distance;
		TimeToAnswer = Time;
		Answer = Correctness;
	}

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float DistanceBetweenObjects;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool Answer;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float TimeToAnswer;
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
	USceneComponent* LeftMeshHolder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RightMeshHolder;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* LeftMesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* RightMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int TrialNumber=0;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StreakMultiplier = 1;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bLastResponse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float CurrentDistance = 1.f;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UStaticMesh* MeshToUse;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UDataTable* History;

	FDateTime LastResponseTimeStamp;

	bool bIsCleanserTrial;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent);
#endif
	UFUNCTION(BlueprintCallable)
	void PositionMeshes(bool bWasResponseCorrect, bool bWasItCleanserTrial);

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
