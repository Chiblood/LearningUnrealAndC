// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MainMenuButton.generated.h"

// ============================================================
//  Forward declarations — avoid including full headers here.
//  The compiler only needs to know these classes exist at this
//  point; full definitions are included in the .cpp file.
// ============================================================
class UButton;
class UTextBlock;

// ============================================================
//  Delegate Declaration
//
//  DECLARE_DYNAMIC_MULTICAST_DELEGATE defines a delegate type
//  that:
//   • Is "Dynamic"    — can be serialized and called from Blueprint
//   • Is "Multicast"  — multiple listeners can subscribe at once
//
//  Naming convention: F<Name> for the type, On<Name> for the property.
// ============================================================
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMainMenuButtonClicked);

/**
 * UMainMenuButton
 *
 * A self-contained, modular button widget for the main menu.
 * Place this inside a parent widget (e.g. UMainMenuWidget) and
 * configure its label in the Blueprint designer.
 *
 * ── Modularity ───────────────────────────────────────────────
 * Rather than hard-coding button labels or logic in the parent
 * widget, each button is its own widget that:
 *   1. Owns a UTextBlock (the label) and a UButton (the click target).
 *   2. Broadcasts OnButtonClicked so any parent can react without
 *      needing to know the internals of this widget.
 *
 * ── BindWidget (meta specifier) ──────────────────────────────
 * meta=(BindWidget) tells UE that the named UPROPERTY must match
 * a child widget with the *exact same variable name* inside the
 * Blueprint subclass.  Mismatch → compile error, which keeps the
 * C++ contract and the visual designer in sync.
 *
 * ── PC vs Mobile ─────────────────────────────────────────────
 * Button sizing and touch padding should be set in the Blueprint.
 * The C++ side stays platform-agnostic; the designer drives layout.
 */
UCLASS(Abstract, Blueprintable, BlueprintType)
class LEARNINGUNREALANDC_API UMainMenuButton : public UUserWidget
{
	GENERATED_BODY()

public:
	// ── Bound Widgets ────────────────────────────────────────
	// These must exist (same variable name) in every Blueprint
	// that subclasses UMainMenuButton.

	/** The UButton that handles click/touch input. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu Button", meta = (BindWidget))
	TObjectPtr<UButton> ButtonWidget;

	/** The text displayed on the button face. */
	UPROPERTY(BlueprintReadOnly, Category = "Main Menu Button", meta = (BindWidget))
	TObjectPtr<UTextBlock> ButtonLabel;

	// ── Events ───────────────────────────────────────────────

	/**
	 * Broadcast whenever the button is clicked or tapped.
	 * BlueprintAssignable — Blueprint event graphs can bind callbacks here.
	 * Parent widgets subscribe to this instead of reaching into button internals.
	 */
	UPROPERTY(BlueprintAssignable, Category = "Main Menu Button|Events")
	FOnMainMenuButtonClicked OnButtonClicked;

	// ── Public API ───────────────────────────────────────────

	/**
	 * Set the display text of this button at runtime.
	 * Useful when buttons are created procedurally rather than laid out
	 * manually in the designer.
	 *
	 * @param NewLabel  The text to show on the button face.
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu Button")
	void SetButtonLabel(const FText& NewLabel);

	/**
	 * Enable or disable this button.
	 * Example usage: disable "Continue" when no save file exists.
	 *
	 * @param bIsEnabled  Pass false to grey out and block input.
	 */
	UFUNCTION(BlueprintCallable, Category = "Main Menu Button")
	void SetButtonEnabled(bool bIsEnabled);

protected:
	// ── Widget Lifecycle ─────────────────────────────────────

	/**
	 * NativePreConstruct runs in the Editor preview (and at runtime before
	 * the widget is fully initialized).  Use it to set default values
	 * visible in the designer — e.g. the default label text.
	 */
	virtual void NativePreConstruct() override;

	/**
	 * NativeConstruct runs once when the widget is fully initialized at
	 * runtime.  This is the right place to bind delegates to child widgets.
	 */
	virtual void NativeConstruct() override;

private:
	// ── Internal Handlers ────────────────────────────────────

	/**
	 * Called by the UButton's internal OnClicked delegate.
	 * Re-broadcasts as OnButtonClicked so parent widgets get a clean,
	 * UMainMenuButton-typed event instead of raw Slate input.
	 *
	 * Must be a UFUNCTION so AddDynamic can register it as a callback.
	 */
	UFUNCTION()
	void HandleButtonClicked();
};
