// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetInteractionComponent.h"
#include "MouseInteractionComponent.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTCONTROL_API UMouseInteractionComponent : public UWidgetInteractionComponent
{
	GENERATED_BODY()
	UMouseInteractionComponent(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	virtual void TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	virtual void PressPointerKey(FKey Key) override;
};
