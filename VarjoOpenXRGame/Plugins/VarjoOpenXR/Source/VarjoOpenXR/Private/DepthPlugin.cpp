// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#include "DepthPlugin.h"
#include "VarjoOpenXR.h"

namespace VarjoOpenXR
{

    void FDepthPlugin::Register()
    {
        RegisterOpenXRExtensionModularFeature();
    }

    void FDepthPlugin::Unregister()
    {
        UnregisterOpenXRExtensionModularFeature();
    }

    bool FDepthPlugin::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
    {
        OutExtensions.Add(XR_VARJO_COMPOSITION_LAYER_DEPTH_TEST_EXTENSION_NAME);
        return true;
    }

    bool FDepthPlugin::GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions)
    {
        OutExtensions.Add(XR_VARJO_ENVIRONMENT_DEPTH_ESTIMATION_EXTENSION_NAME);
        return true;
    }

    void FDepthPlugin::PostCreateInstance(XrInstance InInstance)
    {
        bDepthTestSupported = true;

        XrResult result = xrGetInstanceProcAddr(InInstance, "xrSetEnvironmentDepthEstimationVARJO", reinterpret_cast<PFN_xrVoidFunction*>(&xrSetEnvironmentDepthEstimationVARJO));
        if (result == XR_ERROR_FUNCTION_UNSUPPORTED)
        {
            xrSetEnvironmentDepthEstimationVARJO = nullptr;
        }

        XR_ENSURE(result);
    }

    const void* FDepthPlugin::OnBeginSession(XrSession InSession, const void* InNext)
    {
        Session = InSession;
        State = {};

        return InNext;
    }

    const void* FDepthPlugin::OnEndProjectionLayer(XrSession InSession, int32 InLayerIndex, const void* InNext, XrCompositionLayerFlags& OutFlags)
    {
        if (!State.DepthTestEnabled)
        {
            return InNext;
        }

        DepthTest.type = XR_TYPE_COMPOSITION_LAYER_DEPTH_TEST_VARJO;
        DepthTest.next = InNext;
        DepthTest.depthTestRangeNearZ = State.DepthTestRangeEnabled ? State.DepthTestRangeNearZ : 0.0f;
        DepthTest.depthTestRangeFarZ = State.DepthTestRangeEnabled ? State.DepthTestRangeFarZ : HUGE_VALF;
        return &DepthTest;
    }


    void FDepthPlugin::SetDepthTestEnabled(bool Enabled)
    {
        State.DepthTestEnabled = Enabled;
    }

    bool FDepthPlugin::IsDepthTestSupported()
    {
        return bDepthTestSupported;
    }

    bool FDepthPlugin::IsDepthTestEnabled()
    {
        return bDepthTestSupported && State.DepthTestEnabled;
    }

    void FDepthPlugin::SetDepthTestRange(bool Enabled, float NearZ, float FarZ)
    {
        State.DepthTestRangeEnabled = Enabled;
        State.DepthTestRangeFarZ = FMath::Max(FarZ, FLT_MIN);
        State.DepthTestRangeNearZ = FMath::Clamp(NearZ, 0.0f, State.DepthTestRangeFarZ - FLT_MIN);
    }

    void FDepthPlugin::GetDepthTestRange(bool& IsEnabled, float& NearZ, float& FarZ)
    {
        IsEnabled = State.DepthTestRangeEnabled;
        NearZ = State.DepthTestRangeNearZ;
        FarZ = State.DepthTestRangeFarZ;
    }

    void FDepthPlugin::SetEnvironmentDepthEstimationEnabled(bool Enabled)
    {
        if (xrSetEnvironmentDepthEstimationVARJO == nullptr)
        {
            State.DepthEstimationEnabled = false;
        }
        else
        {
            State.DepthEstimationEnabled = XR_ENSURE(xrSetEnvironmentDepthEstimationVARJO(Session, Enabled)) && Enabled;
        }
    }

    bool FDepthPlugin::IsEnvironmentDepthEstimationSupported()
    {
        return xrSetEnvironmentDepthEstimationVARJO != nullptr;
    }

    bool FDepthPlugin::IsEnvironmentDepthEstimationEnabled()
    {
        return xrSetEnvironmentDepthEstimationVARJO != nullptr && State.DepthEstimationEnabled;
    }
}