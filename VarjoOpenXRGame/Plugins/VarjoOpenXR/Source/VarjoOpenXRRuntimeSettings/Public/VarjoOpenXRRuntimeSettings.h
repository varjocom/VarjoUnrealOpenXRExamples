// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "UObject/Object.h"
#include "Engine/EngineTypes.h"

DECLARE_LOG_CATEGORY_EXTERN(LogVarjoOpenXRSettings, Log, All);

#include "VarjoOpenXRRuntimeSettings.generated.h"


UCLASS(config = Engine, defaultconfig)
class VARJOOPENXRRUNTIMESETTINGS_API UVarjoOpenXRRuntimeSettings : public UObject
{
public:
    GENERATED_BODY()

    UPROPERTY(GlobalConfig, EditAnywhere, Category = "Foveated Rendering", Meta = (DisplayName = "Enable Foveated Rendering", Tooltip = "Enable Foveated Rendering. Please note that this option is usually required for high performance, high quality rendering."))
        bool FoveatedRendering = true;
};
