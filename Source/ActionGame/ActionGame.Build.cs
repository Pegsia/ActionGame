// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ActionGame : ModuleRules
{
	public ActionGame(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicDependencyModuleNames.AddRange(new string[] 
		{	"Core", 
			"CoreUObject", 
			"Engine", 
			"InputCore",
			"AIModule",
			"GameplayTasks",
			"UMG",
			"GameplayTags",
		});

		PublicIncludePaths.AddRange(new string[]
		{
			"ActionGame/Public/Player",
			"ActionGame/Public/Gameplay",
			"ActionGame/Public/Props",
			"ActionGame/Public/Components",
			"ActionGame/Public/System",
			"ActionGame/Public/GameMode",
			"ActionGame/Public/Widget",
			"ActionGame/Public/Actions",
			"ActionGame/Public/AI",
			"ActionGame/Public/AI/Character",
			"ActionGame/Public/AI/Service",
			"ActionGame/Public/AI/Task",
		});

		PrivateDependencyModuleNames.AddRange(new string[]{  });

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });
		
		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
