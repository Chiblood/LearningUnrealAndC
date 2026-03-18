# LearningUnrealAndC

Developed with Unreal Engine 5.6  
A learning project exploring C++ game development and UI systems in Unreal Engine.

---

## Main Menu UI System

Located in `Source/LearningUnrealAndC/UI/`, this system provides a modular,
PC-first (mobile-capable) main menu built entirely in C++ with Blueprint extension points.

### Files

| File | Purpose |
|------|---------|
| `UI/MainMenuButton.h/.cpp` | Reusable single-button widget wrapping `UButton` + `UTextBlock` |
| `UI/MainMenuWidget.h/.cpp` | Root menu widget with Play, Continue, Options, Credits, Quit buttons |
| `UI/MainMenuGameMode.h/.cpp` | GameMode that creates and shows the menu widget on level load |

### How to Wire It Up in the Editor

1. **Create `WBP_MainMenuButton`** — Blueprint subclass of `UMainMenuButton`.
   - In the designer, add a `Button` (variable name: `ButtonWidget`) and a `Text` block (variable name: `ButtonLabel`) inside it.

2. **Create `WBP_MainMenu`** — Blueprint subclass of `UMainMenuWidget`.
   - In the designer, add five `WBP_MainMenuButton` instances with variable names:
     `PlayButton`, `ContinueButton`, `OptionsButton`, `CreditsButton`, `QuitButton`.
   - Wrap everything in a **Safe Zone** widget to handle mobile notch/home-bar offsets.
   - Set `GameLevelName` in the widget Class Defaults to point at your first gameplay level.

3. **Create `BP_MainMenuGameMode`** — Blueprint subclass of `AMainMenuGameMode`.
   - In Class Defaults, assign `MainMenuWidgetClass` → `WBP_MainMenu`.

4. **Assign the GameMode to your main menu level** via *World Settings → GameMode Override*.

### Key C++ Concepts Demonstrated

- `UUserWidget` subclassing with `meta=(BindWidget)` / `meta=(BindWidgetOptional)`
- Multicast delegate declaration (`DECLARE_DYNAMIC_MULTICAST_DELEGATE`) and `AddDynamic`
- `BlueprintImplementableEvent` for Blueprint-extensible hooks
- `TSubclassOf<T>` for type-safe class references
- `CreateWidget<T>` and `AddToViewport`
- `SetInputMode` (UI-only while in menus, game-only during play)
- Save game existence check with `UGameplayStatics::DoesSaveGameExist`
