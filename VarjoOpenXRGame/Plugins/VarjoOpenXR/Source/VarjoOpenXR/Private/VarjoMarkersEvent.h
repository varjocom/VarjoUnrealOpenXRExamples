// Copyright 2021 Varjo Technologies Oy. All rights reserved.

#pragma once

#include "CoreMinimal.h"
#include "UObject/ObjectMacros.h"
#include "Components/ActorComponent.h"

#include "Delegates/Delegate.h"
#include "Delegates/DelegateCombinations.h"

#include "VarjoMarkersEvent.generated.h"


UCLASS(Blueprintable, meta = (BlueprintSpawnableComponent))
class UVarjoMarkersEvent : public UActorComponent
{
    GENERATED_BODY()

public:
    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FNewVarjoMarkerDetected, int32, MarkerId, const FVector&, Position, const FRotator&, Rotation, const FVector2D&, Size);
    UPROPERTY(BlueprintAssignable)
        FNewVarjoMarkerDetected NewVarjoMarkerDetected;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FVarjoMarkerMoved, int32, MarkerId, const FVector&, Position, const FRotator&, Rotation, const FVector2D&, Size);
    UPROPERTY(BlueprintAssignable)
        FVarjoMarkerMoved VarjoMarkerMoved;

    DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FVarjoMarkerLost, int32, MarkerId);
    UPROPERTY(BlueprintAssignable)
        FVarjoMarkerLost VarjoMarkerLost;

    void OnRegister() override;
    void OnUnregister() override;


private:
    void FNewVarjoMarkerDetected_Handler(int32 MarkerId, const FVector& Position, const FRotator& Rotation, const FVector2D& Size) { NewVarjoMarkerDetected.Broadcast(MarkerId, Position, Rotation, Size); }
    void FVarjoMarkerMoved_Handler(int32 MarkerId, const FVector& Position, const FRotator& Rotation, const FVector2D& Size) { VarjoMarkerMoved.Broadcast(MarkerId, Position, Rotation, Size); }
    void FVarjoMarkerLost_Handler(int32 MarkerId) { VarjoMarkerLost.Broadcast(MarkerId); }
};
