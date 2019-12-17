// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "MyGameInstance.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class ECameraMovement : uint8 {

	CM_LOCKED UMETA(DisplayName = "Locked to HMD"),
	CM_FIXED UMETA(DisplayName = "Fixed Camera"),
	CM_BOBBING UMETA(DisplayNane = "Simple camera motion"),
	CM_MAX_CHANGE UMETA(DisplayNane = "Max change")

};

UCLASS()
class ROBOTCONTROL_API UMyGameInstance : public UGameInstance
{
	GENERATED_BODY()
	
public:
	static bool bShadowsEnabled;

	static ECameraMovement CameraMovement;

	static bool bStereoDisplay;


	UFUNCTION(BlueprintCallable)
	static void SetCameraMovement(ECameraMovement Movement);

	UFUNCTION(BlueprintCallable)
	static ECameraMovement GetCameraMovement();

	UFUNCTION(BlueprintCallable)
	static bool GetShadows();
	
	UFUNCTION(BlueprintCallable)
	static void SetShadows(bool Value);

};
