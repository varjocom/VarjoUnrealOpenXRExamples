// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "VarjoOpenXRRuntimeSettings.h"
#include "Modules/ModuleInterface.h"
#include "ISettingsModule.h"
#include "Modules/ModuleManager.h"
#include "PropertyEditorModule.h"

#define LOCTEXT_NAMESPACE "FVarjoOpenXREditorModule"

class FVarjoOpenXREditorModule
    : public IModuleInterface
{
    virtual void StartupModule() override
    {
        ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

        if (SettingsModule != nullptr)
        {
            SettingsModule->RegisterSettings("Project", "Plugins", "VarjoOpenXR",
                LOCTEXT("RuntimeSettingsName", "Varjo OpenXR"),
                LOCTEXT("RuntimeSettingsDescription", "Project settings for Varjo OpenXR Extension plugin"),
                GetMutableDefault<UVarjoOpenXRRuntimeSettings>()
            );
        }
    }

    virtual void ShutdownModule() override
    {
        ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");

        if (SettingsModule != nullptr)
        {
            SettingsModule->UnregisterSettings("Project", "Plugins", "VarjoOpenXR");
        }
    }
};

IMPLEMENT_MODULE(FVarjoOpenXREditorModule, VarjoOpenXREditor);

#undef LOCTEXT_NAMESPACE