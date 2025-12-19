// Copyright 2021 Varjo Technologies Oy. All rights reserved.

using UnrealBuildTool;

public class VarjoOpenXR : ModuleRules
{
    public VarjoOpenXR(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
        PrivatePCHHeaderFile = @"Private\OpenXRCommon.h";

        // Workaround to Windows 11 SDK 10.0.26100.0 INFINITY issue (introduced in 25131)
        PublicDefinitions.Add("_UCRT_LEGACY_INFINITY");

        PublicIncludePaths.AddRange(
            new string[] {
                // ... add public include paths required here ...
            }
            );


        PrivateIncludePaths.AddRange(
            new string[] {
                "VarjoOpenXR/Private/External"
            }
            );


        PublicDependencyModuleNames.AddRange(
            new string[]
            {
                "Core",
                // ... add other public dependencies that you statically link with here ...
            }
            );


        PrivateDependencyModuleNames.AddRange(
            new string[]
            {
                "CoreUObject",
                "Engine",
                "InputCore",
                "AugmentedReality",
                "Slate",
                "SlateCore",
                "HeadMountedDisplay",
                "VarjoOpenXRRuntimeSettings"
            }
            );

        PublicDependencyModuleNames.Add("OpenXRHMD");

        // Required to (delayload) link with OpenXR loader.
        AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenXR");

        DynamicallyLoadedModuleNames.AddRange(
            new string[]
            {
                // ... add any modules that your module loads dynamically here ...
            }
            );
    }
}
