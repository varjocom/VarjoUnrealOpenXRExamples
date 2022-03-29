// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "OpenXRCommon.h"
#include "CoreMinimal.h"

namespace VarjoOpenXR
{
    class FAlphaBlendModePlugin : public IOpenXRExtensionPlugin
    {
    public:
        void Register();
        void Unregister();

        /** IOpenXRExtensionPlugin implementation */
        virtual void PostGetSystem(XrInstance InInstance, XrSystemId InSystem) override;

    private:
        void TryPrioritizeEnvironmentBlendMode(XrInstance InInstance, XrEnvironmentBlendMode InBlendMode);
    };
}