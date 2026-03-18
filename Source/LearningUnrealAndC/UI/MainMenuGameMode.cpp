// Copyright Epic Games, Inc. All Rights Reserved.

#include "UI/MainMenuGameMode.h"
#include "UI/MainMenuWidget.h"

// Blueprint/UserWidget.h provides CreateWidget<T> and the UUserWidget API.
#include "Blueprint/UserWidget.h"

// GameFramework/PlayerController.h is needed to obtain the local player controller,
// which is required by CreateWidget to associate the widget with a player.
#include "GameFramework/PlayerController.h"

// ================================================================
//  Constructor
// ================================================================
AMainMenuGameMode::AMainMenuGameMode()
{
	// MainMenuWidgetClass starts null.  Set it in the Blueprint Class Defaults
	// so the designer can swap widget classes without recompiling.
	MainMenuWidgetClass = nullptr;
}

// ================================================================
//  BeginPlay
// ================================================================
void AMainMenuGameMode::BeginPlay()
{
	// Call the parent implementation first — AGameModeBase::BeginPlay()
	// triggers BeginPlay on all other actors in the level.
	Super::BeginPlay();

	// Create and display the main menu.
	ShowMainMenu();
}

// ================================================================
//  ShowMainMenu  (private)
// ================================================================
void AMainMenuGameMode::ShowMainMenu()
{
	// Guard: a widget class must be assigned in the Blueprint defaults.
	// Log a warning so the developer knows exactly what is missing.
	if (!MainMenuWidgetClass)
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AMainMenuGameMode::ShowMainMenu — MainMenuWidgetClass is not set. "
				 "Please assign a widget class in the Blueprint Class Defaults."));
		return;
	}

	// GetWorld() returns the UWorld that owns this GameMode.
	// GetFirstPlayerController() returns the local player's controller,
	// which CreateWidget needs to tie the widget to the correct player.
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (!IsValid(PlayerController))
	{
		UE_LOG(LogTemp, Warning,
			TEXT("AMainMenuGameMode::ShowMainMenu — No PlayerController found. "
				 "The widget cannot be created without one."));
		return;
	}

	// CreateWidget<T> instantiates the widget and associates it with the player.
	// The template parameter (UMainMenuWidget) provides type safety —
	// no explicit cast is needed and the compiler enforces the class hierarchy.
	MainMenuWidgetInstance = CreateWidget<UMainMenuWidget>(PlayerController, MainMenuWidgetClass);

	if (!IsValid(MainMenuWidgetInstance))
	{
		UE_LOG(LogTemp, Error,
			TEXT("AMainMenuGameMode::ShowMainMenu — CreateWidget returned null. "
				 "Check that MainMenuWidgetClass is a valid UMainMenuWidget subclass."));
		return;
	}

	// AddToViewport renders the widget on top of the 3-D scene.
	// ZOrder 0 is the default layer; increase it if other widgets should appear beneath this one.
	MainMenuWidgetInstance->AddToViewport(0);

	// Show the mouse cursor so the player can click buttons.
	// On mobile, the OS handles touch input regardless of this setting.
	PlayerController->bShowMouseCursor = true;

	// SetInputMode restricts input to the UI only (keyboard/mouse go to the widget,
	// not to a pawn).  UIOnly is appropriate while navigating a menu.
	// Switch back to GameOnly (or GameAndUI) when the game level loads.
	FInputModeUIOnly InputMode;
	InputMode.SetWidgetToFocus(MainMenuWidgetInstance->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	PlayerController->SetInputMode(InputMode);
}
