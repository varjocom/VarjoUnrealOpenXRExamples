// Copyright 2021 Varjo Technologies Oy. All rights reserved.

using UnrealBuildTool;

public class VarjoOpenXREditor : ModuleRules
{
    public VarjoOpenXREditor(ReadOnlyTargetRules Target) : base(Target)
    {
        PrivateDependencyModuleNames.AddRange(
            new string[] {
                "Core",
                "CoreUObject",
                "InputCore",
                "Engine",
                "Slate",
                "SlateCore",
                "EditorStyle",
                "EditorWidgets",
                "DesktopWidgets",
                "PropertyEditor",
                "UnrealEd",
                "SharedSettingsWidgets",
                "TargetPlatform",
                "RenderCore",
                "VarjoOpenXRRuntimeSettings"
            }
        );

        PrivateIncludePathModuleNames.AddRange(
            new string[] {
                "Settings"
            }
        );
    }
}