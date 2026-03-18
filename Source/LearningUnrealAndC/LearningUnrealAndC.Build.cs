// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LearningUnrealAndC : ModuleRules
{
	public LearningUnrealAndC(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		// Core engine modules required by every UE5 project
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });

		// UMG (Unreal Motion Graphics) is required for UUserWidget-based UI classes.
		// Slate and SlateCore are lower-level UI building blocks used internally by UMG.
		// Adding them here unlocks the full widget API in C++.
		PublicDependencyModuleNames.AddRange(new string[] { "UMG", "Slate", "SlateCore" });

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
