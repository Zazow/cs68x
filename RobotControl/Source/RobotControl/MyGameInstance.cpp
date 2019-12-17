// Fill out your copyright notice in the Description page of Project Settings.


#include "MyGameInstance.h"

ECameraMovement UMyGameInstance::CameraMovement;
bool UMyGameInstance::bShadowsEnabled;

void UMyGameInstance::SetCameraMovement(ECameraMovement Movement)
{
	UMyGameInstance::CameraMovement = Movement;
}

ECameraMovement UMyGameInstance::GetCameraMovement()
{
	return UMyGameInstance::CameraMovement;
}

bool UMyGameInstance::GetShadows()
{
	return UMyGameInstance::bShadowsEnabled;
}

void UMyGameInstance::SetShadows(bool Value)
{
	UMyGameInstance::bShadowsEnabled = Value;
}
