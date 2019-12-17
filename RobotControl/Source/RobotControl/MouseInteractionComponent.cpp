// Fill out your copyright notice in the Description page of Project Settings.


#include "MouseInteractionComponent.h"

#include "CollisionQueryParams.h"
#include "Components/PrimitiveComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/ArrowComponent.h"
#include "Framework/Application/SlateApplication.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"

#include "Components/WidgetComponent.h"

void UMouseInteractionComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	FHitResult HitResult;

	const FVector WorldLocation = GetComponentLocation();
	const FTransform WorldTransform = GetComponentTransform();
	const FVector WorldDirection = WorldTransform.GetUnitAxis(EAxis::X);

	TArray<UPrimitiveComponent*> PrimitiveChildren;
	GetRelatedComponentsToIgnoreInAutomaticHitTesting(PrimitiveChildren);

	FCollisionQueryParams Params = FCollisionQueryParams::DefaultQueryParam;
	Params.AddIgnoredComponents(PrimitiveChildren);

	HitResult.TraceStart = WorldLocation;
	HitResult.TraceEnd = WorldLocation + (WorldDirection * 5000.f);

	GetWorld()->LineTraceSingleByChannel(HitResult, HitResult.TraceStart, HitResult.TraceEnd, TraceChannel, Params);

	SetCustomHitResult(HitResult);

	UE_LOG(LogTemp, Warning, TEXT("Start Location: %s, End Location: %s"), *HitResult.TraceStart.ToString(), *HitResult.TraceEnd.ToString());

	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

UMouseInteractionComponent::UMouseInteractionComponent(const FObjectInitializer& ObjectInitializer) : Super(ObjectInitializer)
{
	InteractionSource = EWidgetInteractionSource::Custom;
}

void UMouseInteractionComponent::PressPointerKey(FKey Key)
{
	Super::PressPointerKey(Key);
	if (Key.GetFName() == FName(TEXT("LeftMouseButton"))) {
		UPrimitiveComponent* HitComponenet = GetLastHitResult().GetComponent();
		if (HitComponenet) {
			HitComponenet->DispatchOnClicked();
		}
	}
}
