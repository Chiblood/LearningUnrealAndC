// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "MainMenuGameMode.generated.h"

// Forward-declare UMainMenuWidget so the header stays lightweight.
class UMainMenuWidget;

/**
 * AMainMenuGameMode
 *
 * The GameMode for the main menu level.  It is responsible for creating
 * the UMainMenuWidget and adding it to the player's viewport.
 *
 * ── How to Use ────────────────────────────────────────────────
 *  1. Create a Blueprint subclass of AMainMenuGameMode (e.g. BP_MainMenuGameMode).
 *  2. In its Class Defaults, set MainMenuWidgetClass to your Blueprint
 *     subclass of UMainMenuWidget (e.g. WBP_MainMenu).
 *  3. Open (or create) your main menu level, go to World Settings, and
 *     set GameMode Override to BP_MainMenuGameMode.
 *  4. Play the level — the widget will appear automatically.
 *
 * ── Why a Separate GameMode? ──────────────────────────────────
 * Each level can have its own GameMode.  Keeping main menu logic in a
 * dedicated GameMode prevents it from leaking into gameplay levels and
 * makes the code easier to navigate.
 *
 * ── PC vs Mobile ─────────────────────────────────────────────
 * AddToViewport() works on all platforms.  The widget's Blueprint layout
 * handles platform-specific sizing and safe-zone offsets.
 */
UCLASS(Blueprintable)
class LEARNINGUNREALANDC_API AMainMenuGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AMainMenuGameMode();

	// ── Configuration ────────────────────────────────────────

	/**
	 * The widget class to instantiate as the main menu.
	 *
	 * TSubclassOf<T> is a type-safe class reference — the editor's drop-down
	 * will only show classes derived from UMainMenuWidget.
	 *
	 * Set this in the Blueprint Class Defaults of BP_MainMenuGameMode.
	 */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Main Menu")
	TSubclassOf<UMainMenuWidget> MainMenuWidgetClass;

protected:
	// ── Lifecycle ────────────────────────────────────────────

	/**
	 * BeginPlay is called once when the level is fully loaded and play begins.
	 * This is the correct place to create and show the main menu widget,
	 * because all actors and the player controller are guaranteed to exist.
	 */
	virtual void BeginPlay() override;

private:
	// ── Internal State ───────────────────────────────────────

	/**
	 * Owning pointer to the active main menu widget instance.
	 * UPROPERTY() ensures the garbage collector tracks this reference so the
	 * widget is not destroyed while the game mode holds it.
	 */
	UPROPERTY()
	TObjectPtr<UMainMenuWidget> MainMenuWidgetInstance;

	// ── Helper Functions ─────────────────────────────────────

	/**
	 * Creates the widget from MainMenuWidgetClass and adds it to the viewport.
	 * Extracted into a helper to keep BeginPlay readable.
	 */
	void ShowMainMenu();
};
