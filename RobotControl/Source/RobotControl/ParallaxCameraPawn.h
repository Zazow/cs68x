// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Engine/StaticMeshActor.h"
#include "Camera/CameraComponent.h"
#include "ParallaxCameraPawn.generated.h"

UCLASS()
class ROBOTCONTROL_API AParallaxCameraPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AParallaxCameraPawn();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCameraComponent* Camera;

	UFUNCTION(BlueprintCallable)
	float GetImageSpaceSumDistance();

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float InterpSpeed = 0.2;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector CenterPoint;
	int InterpolationIndex = 0;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FVector> InterpolationPoints;



protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
