// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include <External/openxr/openxr.h>
#include <External/openxr/openxr_platform.h>
#include <External/openxr/openxr_reflection.h>

#include "OpenXRCore.h"

constexpr const TCHAR* OpenXREnvironmentBlendModeToString(XrEnvironmentBlendMode e)
{
    switch (e)
    {
        XR_LIST_ENUM_XrEnvironmentBlendMode(XR_ENUM_CASE_STR);
        default: return TEXT("XrEnvironmentBlendMode_Unknown");
    }
}

#include "IOpenXRHMDModule.h"
#include "IOpenXRExtensionPlugin.h"