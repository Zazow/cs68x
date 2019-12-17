// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "ParallaxCameraComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTCONTROL_API UParallaxCameraComponent : public UCameraComponent
{
	GENERATED_BODY()

	float GetDistancesSumInImageSpace();

	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	

};
