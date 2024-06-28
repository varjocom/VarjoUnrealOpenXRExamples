// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "VarjoOpenXR.h"

#include "MixedRealityPlugin.h"
#include "DepthPlugin.h"
#include "FoveatedRenderingPlugin.h"
#include "VarjoMarkersPlugin.h"
#include "VarjoController.h"

#include "Misc/EngineVersionComparison.h"
#include "HAL/IConsoleManager.h"

DEFINE_LOG_CATEGORY(LogVarjoOpenXR);

#define LOCTEXT_NAMESPACE "FVarjoOpenXRModule"

namespace VarjoOpenXR
{
    static class FVarjoOpenXRModule* g_VarjoOpenXRModule;

    class FVarjoOpenXRModule : public IModuleInterface
    {
    public:
        FMixedRealityPlugin MixedRealityPlugin;
        FDepthPlugin DepthPlugin;
        FFoveatedRenderingPlugin FoveatedRenderingPlugin;
        FVarjoMarkersPlugin VarjoMarkersPlugin;
        FVarjoController VarjoControllerPlugin;

        void StartupModule() override
        {
            MixedRealityPlugin.Register();
            DepthPlugin.Register();
            FoveatedRenderingPlugin.Register();
            VarjoMarkersPlugin.Register();
            VarjoControllerPlugin.Register();

            g_VarjoOpenXRModule = this;


            // This is a workaround for bug in FOpenXRHMDModule that appeared in 5.4.0
            // and is fixed for 5.4.2 in the following commit.
            // https://github.com/EpicGames/UnrealEngine/commit/265cf72abb1f40855c50a64d53b7a13272c97180
            
            // If running version 5.4.0 or 5.4.1
            #if UE_VERSION_NEWER_THAN(5, 4, -1) && UE_VERSION_OLDER_THAN(5, 4, 2)
            IConsoleVariable* setting = IConsoleManager::Get().FindConsoleVariable(TEXT("xr.RetainPreInitInstance"));
            setting->Set(TEXT("True"), ECVF_SetByProjectSetting);
            #endif
        }

        void ShutdownModule() override
        {
            g_VarjoOpenXRModule = nullptr;

            VarjoMarkersPlugin.Unregister();
            FoveatedRenderingPlugin.Unregister();
            DepthPlugin.Unregister();
            MixedRealityPlugin.Unregister();
            VarjoControllerPlugin.Unregister();
        }
    };
}

bool UVarjoOpenXRFunctionLibrary::IsMixedRealitySupported()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->MixedRealityPlugin.IsMixedRealitySupported();
}

bool UVarjoOpenXRFunctionLibrary::IsMixedRealityEnabled()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->MixedRealityPlugin.IsMixedRealityEnabled();
}

bool UVarjoOpenXRFunctionLibrary::SetViewOffset(float Offset)
{
    return VarjoOpenXR::g_VarjoOpenXRModule->MixedRealityPlugin.SetViewOffset(Offset);
}

void UVarjoOpenXRFunctionLibrary::GetViewOffset(float& Offset)
{
    VarjoOpenXR::g_VarjoOpenXRModule->MixedRealityPlugin.GetViewOffset(Offset);
}

void UVarjoOpenXRFunctionLibrary::SetDepthTestEnabled(bool Enabled)
{
    VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.SetDepthTestEnabled(Enabled);
}

bool UVarjoOpenXRFunctionLibrary::IsDepthTestSupported()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.IsDepthTestSupported();
}

bool UVarjoOpenXRFunctionLibrary::IsDepthTestEnabled()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.IsDepthTestEnabled();
}

void UVarjoOpenXRFunctionLibrary::SetDepthTestRange(bool Enabled, float NearZ, float FarZ)
{
    VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.SetDepthTestRange(Enabled, NearZ, FarZ);
}

void UVarjoOpenXRFunctionLibrary::GetDepthTestRange(bool& bIsEnabled, float& NearZ, float& FarZ)
{
    VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.GetDepthTestRange(bIsEnabled, NearZ, FarZ);
}

void UVarjoOpenXRFunctionLibrary::SetEnvironmentDepthEstimationEnabled(bool Enabled)
{
    VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.SetEnvironmentDepthEstimationEnabled(Enabled);
}

bool UVarjoOpenXRFunctionLibrary::IsEnvironmentDepthEstimationSupported()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.IsEnvironmentDepthEstimationSupported();
}

bool UVarjoOpenXRFunctionLibrary::IsEnvironmentDepthEstimationEnabled()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->DepthPlugin.IsEnvironmentDepthEstimationEnabled();
}

bool UVarjoOpenXRFunctionLibrary::IsFoveatedRenderingSupported()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->FoveatedRenderingPlugin.IsFoveatedRenderingSupported();
}

bool UVarjoOpenXRFunctionLibrary::IsFoveatedRenderingEnabled()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->FoveatedRenderingPlugin.IsFoveatedRenderingEnabled();
}

bool UVarjoOpenXRFunctionLibrary::IsVarjoMarkersSupported()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.IsVarjoMarkersSupported();
}

bool UVarjoOpenXRFunctionLibrary::SetVarjoMarkersEnabled(bool enabled)
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.SetVarjoMarkersEnabled(enabled);
}

bool UVarjoOpenXRFunctionLibrary::IsVarjoMarkersEnabled()
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.IsVarjoMarkersEnabled();
}

bool UVarjoOpenXRFunctionLibrary::SetMarkerTimeout(int32 MarkerId, float Duration)
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.SetMarkerTrackingTimeout(MarkerId, Duration);
}

bool UVarjoOpenXRFunctionLibrary::SetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode TrackingMode)
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.SetMarkerTrackingMode(MarkerId, TrackingMode);
}

bool UVarjoOpenXRFunctionLibrary::GetMarkerTrackingMode(int32 MarkerId, EMarkerTrackingMode& TrackingMode)
{
    return VarjoOpenXR::g_VarjoOpenXRModule->VarjoMarkersPlugin.GetMarkerTrackingMode(MarkerId, TrackingMode);
}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(VarjoOpenXR::FVarjoOpenXRModule, VarjoOpenXR)

