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
