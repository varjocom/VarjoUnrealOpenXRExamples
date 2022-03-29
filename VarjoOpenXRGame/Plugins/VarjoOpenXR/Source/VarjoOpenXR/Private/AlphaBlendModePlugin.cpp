// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "AlphaBlendModePlugin.h"

#include "Misc/ConfigCacheIni.h"
#include "VarjoOpenXR.h"
#include "VarjoOpenXRRuntimeSettings.h"

namespace VarjoOpenXR
{
    typedef XrResult(XRAPI_PTR* PFN_xrPrioritizeEnvironmentBlendModeVARJO)(XrInstance instance, XrEnvironmentBlendMode blendMode);

    void FAlphaBlendModePlugin::Register()
    {
        RegisterOpenXRExtensionModularFeature();
    }

    void FAlphaBlendModePlugin::Unregister()
    {
        UnregisterOpenXRExtensionModularFeature();
    }

    void FAlphaBlendModePlugin::PostGetSystem(XrInstance InInstance, XrSystemId InSystem)
    {
        // We can't use UVarjoOpenXRRuntimeSettings default object here, because this is called so early in the Engine lifetime that default objects don't exist yet
        check(GConfig&& GConfig->IsReadyForUse());
        FString modeName;
        if (GConfig->GetString(TEXT("/Script/VarjoOpenXRRuntimeSettings.VarjoOpenXRRuntimeSettings"), TEXT("PreferredEnvironmentBlendMode"), modeName, GEngineIni))
        {
            if (modeName.Equals("Opaque"))
            {
                TryPrioritizeEnvironmentBlendMode(InInstance, XR_ENVIRONMENT_BLEND_MODE_OPAQUE);
            }
        }
    }

    void FAlphaBlendModePlugin::TryPrioritizeEnvironmentBlendMode(XrInstance InInstance, XrEnvironmentBlendMode InBlendMode)
    {
        PFN_xrPrioritizeEnvironmentBlendModeVARJO xrPrioritizeEnvironmentBlendModeVARJO = nullptr;
        XrResult result = xrGetInstanceProcAddr(InInstance, "xrPrioritizeEnvironmentBlendModeVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrPrioritizeEnvironmentBlendModeVARJO));
        if (XR_SUCCEEDED(result) && xrPrioritizeEnvironmentBlendModeVARJO != nullptr)
        {
            XR_ENSURE(xrPrioritizeEnvironmentBlendModeVARJO(InInstance, InBlendMode));
        }
        else
        {
            UE_LOG(LogVarjoOpenXR, Warning, TEXT("Unable to prioritize environment blend mode %s. Varjo OpenXR runtime is probably not installed or inactive."), OpenXREnvironmentBlendModeToString(InBlendMode));
        }
    }
}