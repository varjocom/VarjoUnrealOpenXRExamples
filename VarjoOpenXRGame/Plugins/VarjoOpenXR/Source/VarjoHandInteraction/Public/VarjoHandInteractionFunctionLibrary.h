// Copyright 2025 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "VarjoHandInteractionFunctionLibrary.generated.h"

UCLASS(ClassGroup = OpenXR)
class UVarjoHandInteractionFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintPure, Category = "Varjo|HandInteraction") 
		static void GetHandInteractionAimPose(bool IsRight, bool& IsValid, FVector& Location, FRotator& Rotation);
	UFUNCTION(BlueprintPure, Category = "Varjo|HandInteraction") 
		static void GetHandInteractionGripPose(bool IsRight, bool& IsValid, FVector& Location, FRotator& Rotation);
};
