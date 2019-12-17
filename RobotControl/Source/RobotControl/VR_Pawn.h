// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Camera/CameraComponent.h"
#include "MotionControllerComponent.h"
#include "MouseInteractionComponent.h"
#include "VR_Pawn.generated.h"

UINTERFACE(BlueprintType)
class ROBOTCONTROL_API UGrabbableActor : public UInterface {
	GENERATED_BODY()

};
class ROBOTCONTROL_API IGrabbableActor {
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void Grab(UMotionControllerComponent* Controller);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnControllerClose();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnControllerFar();
};



UCLASS()
class ROBOTCONTROL_API AVR_Pawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AVR_Pawn();

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCameraComponent* Camera;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bUseGameSettings;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* HandsOrigin;
	
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UMotionControllerComponent* RightController;
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UMouseInteractionComponent* RightControllerMouseInteraction;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UMotionControllerComponent* LeftController;

	void(AVR_Pawn::* CameraMotionUpdater)(float DeltaTime) ;


	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float SimpleParallaxSpeed = 0.4;

	int Direction = 1;
	float alpha = 0;
	float Distance = 100.f;
	void SimpleMotion(float DeltaTime);
	void SimpleMotionLookAt(float DeltaTime);
	void SimpleRotattion(float DeltaTime);

	TArray<void(AVR_Pawn::*)(float DeltaTime)> UpdaterFunctions = { &AVR_Pawn::SimpleMotion, &AVR_Pawn::SimpleMotionLookAt, &AVR_Pawn::SimpleRotattion };

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int UpdaterFunction;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	IGrabbableActor* LeftHandGrabbable;
	USceneComponent* LeftHandGrabbableComponent;

	IGrabbableActor* RightHandGrabbable;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
