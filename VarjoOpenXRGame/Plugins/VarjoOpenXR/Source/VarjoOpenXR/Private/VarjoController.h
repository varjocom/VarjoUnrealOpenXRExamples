// Copyright 2023 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "IOpenXRExtensionPlugin.h"

namespace VarjoOpenXR
{
    class FVarjoController :
        public IOpenXRExtensionPlugin
    {
    public:
        FVarjoController() { }

        void Register();
        void Unregister();

        virtual FString GetDisplayName() override
        {
            return FString(TEXT("Varjo Controller"));
        }

        bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        void PostCreateInstance(XrInstance InInstance) override;
        bool GetInteractionProfile(XrInstance InInstance, FString& OutKeyPrefix, XrPath& OutPath, bool& OutHasHaptics) override;
        bool GetControllerModel(XrInstance InInstance, XrPath InInteractionProfile, XrPath InDevicePath, FSoftObjectPath& OutPath) override;
        void GetControllerModelsForCooking(TArray<FSoftObjectPath>& OutPaths) override;

    private:
        XrPath InteractionProfile;
        TMap<XrPath, FSoftObjectPath> ControllerModels;
    };

}

