// Fill out your copyright notice in the Description page of Project Settings.


#include "ParallaxCameraActor.h"
#include "EngineUtils.h"

float AParallaxCameraActor::GetDistancesSumInImageSpace()
{
	// loop through all actors in world:
	for (TObjectIterator<AActor> Itr; Itr; ++Itr)
	{
		// Access the subclass instance with the * or -> operators.
		AActor* Actor = *Itr;



	}
	return 0.0f;
}
