// Copyright 2022 Varjo Technologies Oy. All rights reserved.

#include "MixedRealityPlugin.h"
#include "VarjoOpenXR.h"

#include "IHeadMountedDisplay.h"
#include "IXRTrackingSystem.h"
#include "EngineGlobals.h"
#include "Engine/Engine.h"

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

    bool FMixedRealityPlugin::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
    {
        OutExtensions.Add(XR_VARJO_VIEW_OFFSET_EXTENSION_NAME);
        return true;
    }

    void FMixedRealityPlugin::PostCreateInstance(XrInstance InInstance)
    {
        XrResult result = xrGetInstanceProcAddr(InInstance, "xrSetViewOffsetVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetViewOffsetVARJO));
        if (result == XR_ERROR_FUNCTION_UNSUPPORTED)
        {
            xrSetViewOffsetVARJO = nullptr;
            return;
        }

        XR_ENSURE(result);
    }

    const void* FMixedRealityPlugin::OnBeginSession(XrSession InSession, const void* InNext)
    {
        Session = InSession;
        return InNext;
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

        int32 SystemFlags = 0;

        if (GEngine->XRSystem.IsValid())
        {
            SystemFlags = GEngine->XRSystem->GetXRSystemFlags();
        }

        const bool bMixedRealityEnabled = EXRSystemFlags::IsAR == (SystemFlags & EXRSystemFlags::IsAR);

        // Return true if selected environment blend mode is alpha blend or if override is used and set to alpha blend.
        return (BlendModeOverride ? BlendModeOverride == 3 : bMixedRealityEnabled);
    }

    bool FMixedRealityPlugin::SetViewOffset(float Offset)
    {
        if (xrSetViewOffsetVARJO == nullptr )
        {
            return false;
        }
        else
        {
            if (XR_SUCCEEDED(xrSetViewOffsetVARJO(Session, Offset)))
            {
                ViewOffset = Offset;
                bViewOffsetSet = true;
                return true;
            }
            return false;
        }
    }

    void FMixedRealityPlugin::GetViewOffset(float& Offset)
    {
        if (!bViewOffsetSet)
        {
            Offset = IsMixedRealityEnabled() ? 1.0f : 0.0f;
        }
        else
        {
            Offset = ViewOffset;
        }
    }
}