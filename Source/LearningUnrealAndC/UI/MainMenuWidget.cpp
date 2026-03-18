// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/MainMenuWidget.h"
#include "UI/MainMenuButton.h"

// Kismet/GameplayStatics provides high-level utility functions such as
// OpenLevel, SaveGameToSlot, DoesSaveGameExist, and QuitGame.
#include "Kismet/GameplayStatics.h"

// ================================================================
//  NativeConstruct
// ================================================================
void UMainMenuWidget::NativeConstruct()
{
	// Always call the parent first so UUserWidget can finish its setup.
	Super::NativeConstruct();

	// Wire up each button and decide whether Continue should be active.
	BindButtonDelegates();
	RefreshContinueButtonState();
}

// ================================================================
//  BindButtonDelegates  (private)
// ================================================================
void UMainMenuWidget::BindButtonDelegates()
{
	// For each button we check IsValid() before binding.
	// Because BindWidgetOptional is used, a button may be nullptr if the
	// Blueprint designer hasn't placed it in the layout yet.
	// This guard keeps the menu functional even during incremental setup.

	if (IsValid(PlayButton))
	{
		// AddDynamic registers HandlePlayClicked as a listener on PlayButton's
		// OnButtonClicked multicast delegate.  The macro verifies at compile
		// time that HandlePlayClicked is a UFUNCTION.
		PlayButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::HandlePlayClicked);
	}

	if (IsValid(ContinueButton))
	{
		ContinueButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::HandleContinueClicked);
	}

	if (IsValid(OptionsButton))
	{
		OptionsButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::HandleOptionsClicked);
	}

	if (IsValid(CreditsButton))
	{
		CreditsButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::HandleCreditsClicked);
	}

	if (IsValid(QuitButton))
	{
		QuitButton->OnButtonClicked.AddDynamic(this, &UMainMenuWidget::HandleQuitClicked);
	}
}

// ================================================================
//  RefreshContinueButtonState  (private)
// ================================================================
void UMainMenuWidget::RefreshContinueButtonState()
{
	if (!IsValid(ContinueButton))
	{
		return;
	}

	// DoesSaveGameExist checks the platform's save-game directory for a file
	// with the given slot name.  "SaveSlot_0" is a placeholder — replace it
	// with your actual save-slot name once you implement the save system.
	const bool bHasSave = UGameplayStatics::DoesSaveGameExist(TEXT("SaveSlot_0"), 0);

	// Enable the button only when a save game is present.
	// SetButtonEnabled is our custom helper defined in UMainMenuButton.
	ContinueButton->SetButtonEnabled(bHasSave);
}

// ================================================================
//  HandlePlayClicked  (private)
// ================================================================
void UMainMenuWidget::HandlePlayClicked()
{
	// Fire the Blueprint extension point — Blueprint subclasses can play
	// an intro animation or a screen-transition effect here.
	OnPlayClicked();

	// Only attempt to load a level if one has been configured.
	if (!GameLevelName.IsNone())
	{
		// OpenLevel is the standard UE5 way to transition between maps.
		// GetWorld() returns the UWorld for this widget's context.
		UGameplayStatics::OpenLevel(GetWorld(), GameLevelName);
	}
}

// ================================================================
//  HandleContinueClicked  (private)
// ================================================================
void UMainMenuWidget::HandleContinueClicked()
{
	OnContinueClicked();

	// Use ContinueLevelName if set; otherwise fall back to GameLevelName.
	// A real save system would also restore the player's position and state
	// after the level loads — that logic belongs in the level's GameMode.
	const FName LevelToLoad = ContinueLevelName.IsNone() ? GameLevelName : ContinueLevelName;

	if (!LevelToLoad.IsNone())
	{
		UGameplayStatics::OpenLevel(GetWorld(), LevelToLoad);
	}
}

// ================================================================
//  HandleOptionsClicked  (private)
// ================================================================
void UMainMenuWidget::HandleOptionsClicked()
{
	// The BlueprintImplementableEvent is the primary action here.
	// Typical Blueprint implementation: create and add an options widget,
	// or play an animation that slides the options panel into view.
	OnOptionsClicked();

	// No default C++ action — options UI is best driven from Blueprint
	// since it depends on the project's specific settings structure.
}

// ================================================================
//  HandleCreditsClicked  (private)
// ================================================================
void UMainMenuWidget::HandleCreditsClicked()
{
	OnCreditsClicked();

	// Same pattern as Options — the Blueprint override handles the visual.
}

// ================================================================
//  HandleQuitClicked  (private)
// ================================================================
void UMainMenuWidget::HandleQuitClicked()
{
	// Notify Blueprint first (e.g. show a "Are you sure?" confirmation dialog).
	OnQuitClicked();

	// QuitGame gracefully shuts down the application.
	// On mobile platforms this minimizes the app instead of closing it,
	// which matches platform guidelines.
	// The PlayerController parameter can be nullptr; UE will find one.
	UGameplayStatics::QuitGame(GetWorld(), nullptr, EQuitPreference::Quit, false);
}
