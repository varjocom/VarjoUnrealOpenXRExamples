// Copyright 2021 Varjo Technologies Oy. All rights reserved.

using UnrealBuildTool;

public class VarjoOpenXRRuntimeSettings : ModuleRules
{
    public VarjoOpenXRRuntimeSettings(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                "CoreUObject",
                "Engine"
            }
        );

        if (Target.Type == TargetRules.TargetType.Editor || Target.Type == TargetRules.TargetType.Program)
        {
            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "TargetPlatform"
                }
            );
        }
    }
}
