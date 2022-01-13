// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "OpenXRCommon.h"
#include "CoreMinimal.h"

namespace VarjoOpenXR
{
    class FDepthPlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual bool GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        virtual bool GetOptionalExtensions(TArray<const ANSICHAR*>& OutExtensions) override;
        virtual void PostCreateInstance(XrInstance InInstance) override;
        virtual const void* OnBeginSession(XrSession InSession, const void* InNext) override;
        virtual const void* OnEndProjectionLayer(XrSession InSession, int32 InLayerIndex, const void* InNext, XrCompositionLayerFlags& OutFlags) override;

        /** Plugin functions */
        void SetDepthTestEnabled(bool Enabled);
        bool IsDepthTestSupported();
        bool IsDepthTestEnabled();
        void SetDepthTestRange(bool Enabled, float NearZ, float FarZ);
        void GetDepthTestRange(bool& IsEnabled, float& NearZ, float& FarZ);
        void SetEnvironmentDepthEstimationEnabled(bool Enabled);
        bool IsEnvironmentDepthEstimationSupported();
        bool IsEnvironmentDepthEstimationEnabled();

    private:
        bool bDepthTestSupported = false;
        PFN_xrSetEnvironmentDepthEstimationVARJO xrSetEnvironmentDepthEstimationVARJO = nullptr;

        XrSession Session = XR_NULL_HANDLE;
        XrCompositionLayerDepthTestVARJO DepthTest;

        struct FPluginState
        {
            bool DepthTestEnabled;
            bool DepthTestRangeEnabled;
            float DepthTestRangeNearZ;
            float DepthTestRangeFarZ = 0.75f;
            bool DepthEstimationEnabled;
        };

        FPluginState State;
    };
}