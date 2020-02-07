// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorUtilityWidget.h"
#include "Engine/DataTable.h"
#include "PlaybackWidgetBase.generated.h"

/**
 * 
 */
UCLASS()
class ROBOTCONTROL_API UPlaybackWidgetBase : public UEditorUtilityWidget
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable)
	void OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFileName);

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UDataTable* PlaybackTable;
};
