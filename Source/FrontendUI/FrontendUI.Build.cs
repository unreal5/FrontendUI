// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FrontendUI : ModuleRules
{
	public FrontendUI(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		PrivateDependencyModuleNames.AddRange(new string[] { "CommonUI", "UMG" });
		PublicDependencyModuleNames.AddRange([
			"Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags"
		]);
		PublicIncludePaths.AddRange(["FrontendUI/"]);
	}
}