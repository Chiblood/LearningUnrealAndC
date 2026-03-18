// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuWidget.generated.h"

// Forward-declare the button class so we don't need to pull in its full header here.
class UMainMenuButton;

/**
 * UMainMenuWidget
 *
 * The root widget for the game's main menu screen.  It holds five modular
 * UMainMenuButton instances and routes each button's click to the appropriate
 * game action.
 *
 * ── Standard Buttons ─────────────────────────────────────────
 *   PlayButton     — Start a new game / enter the first level
 *   ContinueButton — Resume the most-recent save (disabled if none)
 *   OptionsButton  — Open the settings / options sub-screen
 *   CreditsButton  — Show the credits sub-screen
 *   QuitButton     — Exit the application
 *
 * ── BindWidgetOptional ────────────────────────────────────────
 * Unlike BindWidget (which causes a compile error if the named
 * child widget is missing), BindWidgetOptional makes the binding
 * optional.  This lets the project compile even before the Blueprint
 * designer has added every button — useful while iterating.
 * Switch individual buttons to BindWidget once the layout is stable.
 *
 * ── PC vs Mobile ─────────────────────────────────────────────
 * All sizing, anchoring and padding are handled in the Blueprint
 * designer so you can tune them per-platform without touching C++.
 * Add a "Safe Zone" widget as the outermost container in Blueprint
 * to handle mobile notch / home-bar areas automatically.
 *
 * ── Extending in Blueprint ───────────────────────────────────
 * BlueprintImplementableEvent functions are empty stubs in C++.
 * Override them in a Blueprint subclass to add transitions,
 * animations, or audio feedback without editing C++ code.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class LEARNINGUNREALANDC_API UMainMenuWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	// ── Bound Button References ──────────────────────────────
	// Each property must match a child widget variable name in the Blueprint.

	/** Starts a new game session. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu|Buttons", meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuButton> PlayButton;

	/** Resumes the most-recent save.  Disabled when no save exists. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu|Buttons", meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuButton> ContinueButton;

	/** Opens the options / settings sub-screen. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu|Buttons", meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuButton> OptionsButton;

	/** Shows the credits sub-screen. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu|Buttons", meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuButton> CreditsButton;

	/** Exits the application. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu|Buttons", meta = (BindWidgetOptional))
	TObjectPtr<UMainMenuButton> QuitButton;

	// ── Configuration ────────────────────────────────────────

	/**
	 * The map/level to open when Play is clicked.
	 * Set this in the Blueprint defaults or in the Level's World Settings.
	 * Example: "/Game/Levels/FirstLevel"
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu|Config")
	FName GameLevelName;

	/**
	 * The map/level to open when Continue is clicked.
	 * This may be the same as GameLevelName if you restore state from a save file.
	 */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Main Menu|Config")
	FName ContinueLevelName;

protected:
	// ── Widget Lifecycle ─────────────────────────────────────

	/**
	 * Called once at runtime when the widget and all its children are ready.
	 * This is where we bind each button's OnButtonClicked delegate.
	 */
	virtual void NativeConstruct() override;

	// ── Blueprint Extension Points ───────────────────────────
	//
	// BlueprintImplementableEvent:
	//   • C++ declares the function signature; Blueprint provides the body.
	//   • If Blueprint doesn't override it, the call is silently skipped.
	//   • Use these to add screen transitions, sound effects, or animations
	//     without touching C++ code.

	/** Called just before the game level is opened. Add a transition animation here. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu|Events")
	void OnPlayClicked();

	/** Called just before resuming a saved game. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu|Events")
	void OnContinueClicked();

	/** Called when the Options button is clicked. Show an options widget here. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu|Events")
	void OnOptionsClicked();

	/** Called when the Credits button is clicked. Show a credits widget here. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu|Events")
	void OnCreditsClicked();

	/** Called just before the application quits. Useful for a confirmation dialog. */
	UFUNCTION(BlueprintImplementableEvent, Category = "Main Menu|Events")
	void OnQuitClicked();

private:
	// ── Internal Click Handlers ──────────────────────────────
	// Each handler fires the BlueprintImplementableEvent and then performs
	// the default C++ action (e.g. loading a level or calling Quit).
	// Must be UFUNCTIONs so AddDynamic can register them as delegate callbacks.

	UFUNCTION()
	void HandlePlayClicked();

	UFUNCTION()
	void HandleContinueClicked();

	UFUNCTION()
	void HandleOptionsClicked();

	UFUNCTION()
	void HandleCreditsClicked();

	UFUNCTION()
	void HandleQuitClicked();

	// ── Helper Functions ─────────────────────────────────────

	/**
	 * Binds OnButtonClicked for every button that was successfully bound
	 * (i.e. is non-null after BindWidgetOptional resolution).
	 */
	void BindButtonDelegates();

	/**
	 * Checks whether a save game exists and enables/disables ContinueButton
	 * accordingly, so the player cannot click it without a valid save.
	 */
	void RefreshContinueButtonState();
};
