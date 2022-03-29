// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "VarjoOpenXR.h"

#include "AlphaBlendModePlugin.h"
#include "DepthPlugin.h"
#include "FoveatedRenderingPlugin.h"
#include "VarjoMarkersPlugin.h"

DEFINE_LOG_CATEGORY(LogVarjoOpenXR);

#define LOCTEXT_NAMESPACE "FVarjoOpenXRModule"

namespace VarjoOpenXR
{
    static class FVarjoOpenXRModule * g_VarjoOpenXRModule;

    class FVarjoOpenXRModule : public IModuleInterface
    {
    public:
        FDepthPlugin DepthPlugin;
        FAlphaBlendModePlugin AlphaBlendModePlugin;
        FFoveatedRenderingPlugin FoveatedRenderingPlugin;
        FVarjoMarkersPlugin VarjoMarkersPlugin;

        void StartupModule() override
        {
            AlphaBlendModePlugin.Register();
            DepthPlugin.Register();
            FoveatedRenderingPlugin.Register();
            VarjoMarkersPlugin.Register();

            g_VarjoOpenXRModule = this;
        }

        void ShutdownModule() override
        {
            g_VarjoOpenXRModule = nullptr;

            VarjoMarkersPlugin.Unregister();
            FoveatedRenderingPlugin.Unregister();
            DepthPlugin.Unregister();
            AlphaBlendModePlugin.Unregister();
        }
    };
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

