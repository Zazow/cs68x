// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ObjectCatcherManager.h"
#include "Components/StaticMeshComponent.h"
#include "BallToCatch.generated.h"

UCLASS()
class ROBOTCONTROL_API ABallToCatch : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABallToCatch();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	AObjectCatcherManager* ManagerObject;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	UMaterialInterface* MaterialToUse;

	UMaterialInstanceDynamic* MaterialInstance;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Gravity;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
