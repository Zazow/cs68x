// Fill out your copyright notice in the Description page of Project Settings.


#include "PlaybackWidgetBase.h"
#include "Engine.h"
#include "Developer/DesktopPlatform/Public/IDesktopPlatform.h"
#include "Developer/DesktopPlatform/Public/DesktopPlatformModule.h"
#include "Interfaces/IMainFrameModule.h"

void UPlaybackWidgetBase::OpenFileDialog(const FString& DialogTitle, const FString& DefaultPath, const FString& FileTypes, FString& OutFileName)
{
	UE_LOG(LogTemp, Warning, TEXT("Opening Dialog"));
	if (GEngine) {

		IMainFrameModule& MainFrameModule = FModuleManager::LoadModuleChecked<IMainFrameModule>(TEXT("MainFrame"));
		const TSharedPtr<SWindow>& MainFrameParentWindow = MainFrameModule.GetParentWindow();
		if (MainFrameParentWindow.IsValid() && MainFrameParentWindow->GetNativeWindow().IsValid())
		{
			void* ParentWindowWindowHandle = MainFrameParentWindow->GetNativeWindow()->GetOSWindowHandle();
			IDesktopPlatform* DesktopPlatform = FDesktopPlatformModule::Get();
			if (DesktopPlatform) {
				// Open file picker:
				uint32 SelectionFlag = 0;
				TArray<FString> OutNames;
				DesktopPlatform->OpenFileDialog(ParentWindowWindowHandle, DialogTitle, DefaultPath, FString(""), FileTypes, SelectionFlag, OutNames);
				if (OutNames.Num() > 0) {
					OutFileName = OutNames[0];
				}
			}
		}
	}
}

void UPlaybackWidgetBase::PopulateData()
{
	Data.Empty();
	TArray<FObjectCatcherHistoryPoint*> Rows;
	PlaybackTable->GetAllRows(FString(""), Rows);
	float CurrentTime = 0.f;

	for (auto Row : Rows) {
		CurrentTime += Row->DeltaTime;
		Data.Add(TPair<float, FObjectCatcherHistoryPoint*>(CurrentTime, Row));
	}
	Duration = CurrentTime;
}

FObjectCatcherHistoryPoint UPlaybackWidgetBase::GetInterpolatedHistoryPoint(float TimeSinceStart)
{
	if (Data.Num() == 0) {
		return FObjectCatcherHistoryPoint();
	}

	auto i = FindLatestPoint(TimeSinceStart);
	if (i == Data.Num() - 1) {
		// there is nothing to interpolate.
		return *Data[i].Value;
	}

	auto A = Data[i].Value;
	auto B = Data[i + 1].Value;
	float alpha = (TimeSinceStart - Data[i].Key) / (Data[i + 1].Key - Data[i].Key);
	check(alpha >= 0 && alpha < 1);

	auto Result = FObjectCatcherHistoryPoint(
		FMath::Lerp<FVector>(A->ControllerPosition, B->ControllerPosition, alpha),
		FMath::Lerp<FVector>(A->BallPosition, B->BallPosition, alpha),
		TimeSinceStart - Data[i].Key,
		A->BallNumber,
		FMath::Lerp<float>(A->RoundScore, B->RoundScore, alpha),
		FMath::Lerp<float>(A->TotalScore, B->TotalScore, alpha),
		FMath::Lerp<float>(A->XYDistanceNormalized, B->XYDistanceNormalized, alpha)
	);
	return Result;
}

int UPlaybackWidgetBase::FindLatestPoint(float Time)
{
	// TODO: Implement binary search
	for (int i = 0; i < Data.Num(); i++) {
		if (Data[i].Key >= Time) {
			// Check to see if given time is not smaller than the first history point.
			if (i - 1 < 0) {
				return INDEX_NONE;
			}
			return i - 1;
		}
	}

	// if all history points are smaller than time, than the last entry is the latest point that is smaller than time.
	return Data.Num() - 1;
}
