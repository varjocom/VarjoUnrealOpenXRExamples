// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVarjoOpenXRSettings, Log, All);

#include "VarjoOpenXRRuntimeSettings.generated.h"

UENUM()
enum class EnvironmentBlendMode
{
    Opaque = 0,
    AlphaBlend = 1
};

UCLASS(config = Engine, defaultconfig)
class VARJOOPENXRRUNTIMESETTINGS_API UVarjoOpenXRRuntimeSettings : public UObject
{
public:
    GENERATED_BODY()

    UPROPERTY(GlobalConfig, EditAnywhere, Category = "Environment Blend Mode", Meta = (ConfigRestartRequired = true, DisplayName = "Preferred Environment Blend Mode (Requires Restart)", Tooltip = "Select 'Opaque' for VR and 'Alpha Blend' for MR. Editor restart required."))
        EnvironmentBlendMode PreferredEnvironmentBlendMode = EnvironmentBlendMode::Opaque;

    UPROPERTY(GlobalConfig, EditAnywhere, Category = "Foveated Rendering", Meta = (DisplayName = "Enable Foveated Rendering", Tooltip = "Enable Foveated Rendering. Please note that this option is usually required for high performance, high quality rendering."))
        bool FoveatedRendering = true;
};
