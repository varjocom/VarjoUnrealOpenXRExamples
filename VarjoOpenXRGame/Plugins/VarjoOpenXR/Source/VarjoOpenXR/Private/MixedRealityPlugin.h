// Copyright 2022 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "OpenXRCommon.h"
#include "CoreMinimal.h"

namespace VarjoOpenXR
{
    class FMixedRealityPlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual void* OnEnumerateViewConfigurationViews(XrInstance InInstance, XrSystemId InSystem, XrViewConfigurationType InViewConfigurationType, uint32_t InViewIndex, void* InNext) override;

        bool IsMixedRealitySupported();
        bool IsMixedRealityEnabled();

    private:
        bool bBlendModesEnumerated = false;
        bool bMixedRealitySupported = false;
        XrEnvironmentBlendMode PrimaryBlendMode;
    };
}