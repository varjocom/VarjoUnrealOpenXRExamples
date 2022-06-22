// Copyright 2022 Varjo Technologies Oy. All rights reserved.

#include "MixedRealityPlugin.h"
#include "VarjoOpenXR.h"

namespace VarjoOpenXR
{
    void FMixedRealityPlugin::Register()
    {
        RegisterOpenXRExtensionModularFeature();
    }

    void FMixedRealityPlugin::Unregister()
    {
        UnregisterOpenXRExtensionModularFeature();
    }

    void* FMixedRealityPlugin::OnEnumerateViewConfigurationViews(XrInstance InInstance, XrSystemId InSystem, XrViewConfigurationType InViewConfigurationType, uint32_t InViewIndex, void* InNext)
    {
        if (bBlendModesEnumerated)
        {
            // We only need to enumerate the environment blend modes once
            return InNext;
        }

        // Enumerate environment blend modes and see if alpha blend mode is supported
        uint32 BlendModeCount;
        TArray<XrEnvironmentBlendMode> BlendModes;

        XR_ENSURE(xrEnumerateEnvironmentBlendModes(InInstance, InSystem, InViewConfigurationType, 0, &BlendModeCount, nullptr));
        // Fill the initial array with valid enum types (this will fail in the validation layer otherwise).
        BlendModes.SetNum(BlendModeCount);
        for (auto& TypeIter : BlendModes)
            TypeIter = XR_ENVIRONMENT_BLEND_MODE_OPAQUE;
        XR_ENSURE(xrEnumerateEnvironmentBlendModes(InInstance, InSystem, InViewConfigurationType, BlendModeCount, &BlendModeCount, BlendModes.GetData()));

        if (BlendModeCount > 0 && BlendModes.Contains(XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND)) {
            bMixedRealitySupported = true;
            PrimaryBlendMode = BlendModes[0];
        }

        bBlendModesEnumerated = true;
        return InNext;
    }

    bool FMixedRealityPlugin::IsMixedRealitySupported()
    {
        return bMixedRealitySupported;
    }

    bool FMixedRealityPlugin::IsMixedRealityEnabled()
    {
        if (!bMixedRealitySupported) return false;

        static const auto CVar = IConsoleManager::Get().FindConsoleVariable(TEXT("xr.OpenXREnvironmentBlendMode"));
        int32 BlendModeOverride = CVar->GetInt();

        // Return true if primary blend mode is alpha blend or if override is used and set to alpha blend.
        return BlendModeOverride ? BlendModeOverride == 3 : PrimaryBlendMode == XR_ENVIRONMENT_BLEND_MODE_ALPHA_BLEND;
    }
}