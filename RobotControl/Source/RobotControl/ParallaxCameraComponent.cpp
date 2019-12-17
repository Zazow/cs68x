// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallaxCameraComponent.h"
#include "EngineUtils.h"

void UParallaxCameraComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	// find the 
}

float UParallaxCameraComponent::GetDistancesSumInImageSpace()
{
	TArray<AActor> VisibleActors;

	// loop through all actors in world:
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		AActor* Actor= *Itr;
	
		
	}

	return 0.0f;
}
