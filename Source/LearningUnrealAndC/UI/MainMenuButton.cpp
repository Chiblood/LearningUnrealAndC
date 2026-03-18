// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/MainMenuButton.h"

// UMG widget includes — needed to call SetText, SetIsEnabled, etc.
#include "Components/Button.h"
#include "Components/TextBlock.h"

// ----------------------------------------------------------------
//  NativePreConstruct
// ----------------------------------------------------------------
void UMainMenuButton::NativePreConstruct()
{
	// Always call the parent implementation first.
	// This ensures the engine can do its own setup before our code runs.
	Super::NativePreConstruct();

	// Nothing custom here for now, but this hook is the correct place
	// to apply designer-time property values (e.g. default label text
	// from an EditAnywhere UPROPERTY) to the bound child widgets.
}

// ----------------------------------------------------------------
//  NativeConstruct
// ----------------------------------------------------------------
void UMainMenuButton::NativeConstruct()
{
	Super::NativeConstruct();

	// Guard: ButtonWidget must exist (enforced by BindWidget at compile time,
	// but a runtime guard is good defensive practice).
	if (ButtonWidget)
	{
		// AddDynamic is an Unreal macro that registers a UFUNCTION as a delegate
		// listener.  When ButtonWidget is clicked/tapped, HandleButtonClicked fires.
		ButtonWidget->OnClicked.AddDynamic(this, &UMainMenuButton::HandleButtonClicked);
	}
}

// ----------------------------------------------------------------
//  SetButtonLabel
// ----------------------------------------------------------------
void UMainMenuButton::SetButtonLabel(const FText& NewLabel)
{
	// ButtonLabel is bound via BindWidget; it is guaranteed non-null if the
	// Blueprint subclass is set up correctly.
	if (ButtonLabel)
	{
		ButtonLabel->SetText(NewLabel);
	}
}

// ----------------------------------------------------------------
//  SetButtonEnabled
// ----------------------------------------------------------------
void UMainMenuButton::SetButtonEnabled(bool bIsEnabled)
{
	if (ButtonWidget)
	{
		// SetIsEnabled propagates to Slate, which handles visual graying
		// as well as blocking mouse/touch input when disabled.
		ButtonWidget->SetIsEnabled(bIsEnabled);
	}
}

// ----------------------------------------------------------------
//  HandleButtonClicked  (private)
// ----------------------------------------------------------------
void UMainMenuButton::HandleButtonClicked()
{
	// Forward the raw UButton click as our own typed broadcast.
	// Subscribers (e.g. UMainMenuWidget) only need to know "a menu
	// button was clicked" — they don't need the UButton reference.
	OnButtonClicked.Broadcast();
}
