// Copyright 2025 Varjo Technologies Oy. All rights reserved.

#include "VarjoHandInteraction.h"
#include "IXRTrackingSystem.h"
#include "OpenXRCore.h"
#include "UObject/UObjectIterator.h"
#include "IOpenXRExtensionPlugin.h"
#include "DrawDebugHelpers.h"
#include "Engine/Engine.h"
#include "Misc/ConfigCacheIni.h"

#include "EnhancedInputDeveloperSettings.h"
#include "EnhancedInputLibrary.h"
#include "EnhancedInputSubsystemInterface.h"
#include "EnhancedInputModule.h"
#include "InputAction.h"
#include "InputMappingContext.h"
#include "PlayerMappableInputConfig.h"
#include "GenericPlatform/GenericPlatformInputDeviceMapper.h"

#include "CoreMinimal.h"
#include "InputCoreTypes.h"

#if WITH_EDITOR
#include "Editor/EditorEngine.h"
#include "Editor.h"
#include "EnhancedInputEditorSubsystem.h"
#endif

#define LOCTEXT_NAMESPACE "FVarjoHandInteraction"

DEFINE_LOG_CATEGORY(LogVarjoHandInteraction);

FVarjoHandInteraction* FVarjoHandInteraction::m_Instance = nullptr;

FVarjoHandInteraction::FVarjoInteractionController::FVarjoInteractionController() 
	: RolePath(XR_NULL_PATH)
	, AimPoseAction(XR_NULL_HANDLE)
	, AimPoseActionPath(XR_NULL_PATH)
	, GripPoseAction(XR_NULL_HANDLE)
	, GripPoseActionPath(XR_NULL_PATH)
	, PinchPoseAction(XR_NULL_HANDLE)
	, PinchPoseActionPath(XR_NULL_PATH)
	, PokePoseAction(XR_NULL_HANDLE)
	, PokePoseActionPath(XR_NULL_PATH)
	, PalmPoseAction(XR_NULL_HANDLE)
	, PalmPoseActionPath(XR_NULL_PATH)
	, AimAction(XR_NULL_HANDLE)
	, AimActionPath(XR_NULL_PATH)
	, GraspAction(XR_NULL_HANDLE)
	, GraspActionPath(XR_NULL_PATH)
	, PinchAction(XR_NULL_HANDLE)
	, PinchActionPath(XR_NULL_PATH)
	, AimReadyAction(XR_NULL_HANDLE)
	, AimReadyActionPath(XR_NULL_PATH)
	, GraspReadyAction(XR_NULL_HANDLE)
	, GraspReadyActionPath(XR_NULL_PATH)
	, PinchReadyAction(XR_NULL_HANDLE)
	, PinchReadyActionPath(XR_NULL_PATH)
	, AimDeviceId(-1)
	, GripDeviceId(-1)
	, PinchDeviceId(-1)
	, PokeDeviceId(-1)
	, PalmDeviceId(-1)
	, AimReadyActionState({ XR_TYPE_ACTION_STATE_BOOLEAN })
	, GraspReadyActionState({ XR_TYPE_ACTION_STATE_BOOLEAN })
	, PinchReadyActionState({ XR_TYPE_ACTION_STATE_BOOLEAN })
	, AimValueActionState({ XR_TYPE_ACTION_STATE_FLOAT })
	, GraspValueActionState({ XR_TYPE_ACTION_STATE_FLOAT })
	, PinchValueActionState({ XR_TYPE_ACTION_STATE_FLOAT })
	, AimKey(TEXT(""))
	, GraspKey(TEXT(""))
	, PinchKey(TEXT(""))
{
	SubactionPaths.RemoveAll([](const int& num) 
	{
		return true;
	});

	AimInputActions.Empty();
	GraspInputActions.Empty();
	PinchInputActions.Empty();
	AimReadyInputActions.Empty();
	GraspReadyInputActions.Empty();
	PinchReadyInputActions.Empty();
}

void FVarjoHandInteraction::FVarjoInteractionController::SetupPath(FString InRolePath, FString InAimPoseActionPath, FString InGripPoseActionPath, FString InPinchPoseActionPath, FString InPokePoseActionPath, FString InPalmPoseActionPath, FString InAimActionPath, FString InGraspActionPath, FString InPinchActionPath, FString InAimReadyActionPath, FString InGraspReadyActionPath, FString InPinchReadyActionPath)
{
	RolePath = FOpenXRPath(InRolePath);
	AimPoseActionPath = FOpenXRPath(InAimPoseActionPath);
	GripPoseActionPath = FOpenXRPath(InGripPoseActionPath);
	PinchPoseActionPath = FOpenXRPath(InPinchPoseActionPath);
	PokePoseActionPath = FOpenXRPath(InPokePoseActionPath);
	PalmPoseActionPath = FOpenXRPath(InPalmPoseActionPath);
	AimActionPath = FOpenXRPath(InAimActionPath);
	GraspActionPath = FOpenXRPath(InGraspActionPath);
	PinchActionPath = FOpenXRPath(InPinchActionPath);
	AimReadyActionPath = FOpenXRPath(InAimReadyActionPath);
	GraspReadyActionPath = FOpenXRPath(InGraspReadyActionPath);
	PinchReadyActionPath = FOpenXRPath(InPinchReadyActionPath);

	SubactionPaths.Reset();
	SubactionPaths.Emplace(RolePath);
}

int32 FVarjoHandInteraction::FVarjoInteractionController::AddTrackedDevices(FOpenXRHMD* HMD)
{
	if (HMD)
	{
		AimDeviceId = HMD->AddTrackedDevice(AimPoseAction, AimPoseActionPath);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("AddTrackedDevices ID: %d"), AimDeviceId);
		GripDeviceId = HMD->AddTrackedDevice(GripPoseAction, GripPoseActionPath);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("AddTrackedDevices ID: %d"), GripDeviceId);
		PinchDeviceId = HMD->AddTrackedDevice(PinchPoseAction, PinchPoseActionPath);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("AddTrackedDevices ID: %d"), PinchDeviceId);
		PokeDeviceId = HMD->AddTrackedDevice(PokePoseAction, PokePoseActionPath);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("AddTrackedDevices ID: %d"), PokeDeviceId);
		PalmDeviceId = HMD->AddTrackedDevice(PalmPoseAction, PalmPoseActionPath);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("AddTrackedDevices ID: %d"), PalmDeviceId);
	}
	return AimDeviceId;
}

void FVarjoHandInteraction::FVarjoInteractionController::GetSuggestedBindings(TArray<XrActionSuggestedBinding>& OutSuggestedBindings)
{
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ AimPoseAction, AimPoseActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ GripPoseAction, GripPoseActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ GraspAction, GraspActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ PinchAction, PinchActionPath });

	OutSuggestedBindings.Add(XrActionSuggestedBinding{ AimAction, AimActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ PinchPoseAction, PinchPoseActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ PokePoseAction, PokePoseActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ PalmPoseAction, PalmPoseActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ AimReadyAction, AimReadyActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ GraspReadyAction, GraspReadyActionPath });
	OutSuggestedBindings.Add(XrActionSuggestedBinding{ PinchReadyAction, PinchReadyActionPath });
}

void FVarjoHandInteraction::FVarjoInteractionController::AddAction(XrActionSet& InActionSet, XrAction& OutAction, FOpenXRPath InBindingPath, XrActionType InActionType)
{
	check(InActionSet != XR_NULL_HANDLE);
	if (OutAction != XR_NULL_HANDLE) 
	{
		xrDestroyAction(OutAction);
		OutAction = XR_NULL_HANDLE;
	}
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("xrCreateAction %s"), *InBindingPath.ToString());

	char ActionName[NAME_SIZE];
	GetActionName(InBindingPath).GetPlainANSIString(ActionName);

	XrActionCreateInfo Info;
	Info.type = XR_TYPE_ACTION_CREATE_INFO;
	Info.next = nullptr;
	Info.actionType = InActionType;
	Info.countSubactionPaths = SubactionPaths.Num();
	Info.subactionPaths = SubactionPaths.GetData();
	FCStringAnsi::Strcpy(Info.actionName, XR_MAX_ACTION_NAME_SIZE, ActionName);
	FCStringAnsi::Strcpy(Info.localizedActionName, XR_MAX_LOCALIZED_ACTION_NAME_SIZE, ActionName);
	XR_ENSURE(xrCreateAction(InActionSet, &Info, &OutAction));
}

void FVarjoHandInteraction::FVarjoInteractionController::AddActions(XrActionSet& InActionSet)
{
	AddAction(InActionSet, AimPoseAction, AimPoseActionPath, XR_ACTION_TYPE_POSE_INPUT);
	AddAction(InActionSet, GripPoseAction, GripPoseActionPath, XR_ACTION_TYPE_POSE_INPUT);
	AddAction(InActionSet, GraspAction, GraspActionPath, XR_ACTION_TYPE_FLOAT_INPUT);
	AddAction(InActionSet, PinchAction, PinchActionPath, XR_ACTION_TYPE_FLOAT_INPUT);

	AddAction(InActionSet, PinchPoseAction, PinchPoseActionPath, XR_ACTION_TYPE_POSE_INPUT);
	AddAction(InActionSet, PokePoseAction, PokePoseActionPath, XR_ACTION_TYPE_POSE_INPUT);
	AddAction(InActionSet, PalmPoseAction, PalmPoseActionPath, XR_ACTION_TYPE_POSE_INPUT);
	AddAction(InActionSet, AimAction, AimActionPath, XR_ACTION_TYPE_FLOAT_INPUT);
	AddAction(InActionSet, AimReadyAction, AimReadyActionPath, XR_ACTION_TYPE_BOOLEAN_INPUT);
	AddAction(InActionSet, GraspReadyAction, GraspReadyActionPath, XR_ACTION_TYPE_BOOLEAN_INPUT);
	AddAction(InActionSet, PinchReadyAction, PinchReadyActionPath, XR_ACTION_TYPE_BOOLEAN_INPUT);
}

FName FVarjoHandInteraction::FVarjoInteractionController::GetActionName(FOpenXRPath ActionPath)
{
	TArray<FString> Tokens;
	ActionPath.ToString().ParseIntoArray(Tokens, TEXT("/"));
	FString ActionNameString = Tokens[2] + "_" + Tokens[4] + "_" + Tokens[5];
	return FName(ActionNameString);
}

void FVarjoHandInteraction::FVarjoInteractionController::SyncActionStates(XrSession InSession)
{
	XrActionStateGetInfo AimActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	AimActionStateInfo.action = AimAction;
	XR_ENSURE(xrGetActionStateFloat(InSession, &AimActionStateInfo, &AimValueActionState));

	XrActionStateGetInfo PinchReadyActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	PinchReadyActionStateInfo.action = PinchReadyAction;
	XR_ENSURE(xrGetActionStateBoolean(InSession, &PinchReadyActionStateInfo, &PinchReadyActionState));

	XrActionStateGetInfo AimReadyActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	AimReadyActionStateInfo.action = AimReadyAction;
	XR_ENSURE(xrGetActionStateBoolean(InSession, &AimReadyActionStateInfo, &AimReadyActionState));

	XrActionStateGetInfo GraspReadyActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	GraspReadyActionStateInfo.action = GraspReadyAction;
	XR_ENSURE(xrGetActionStateBoolean(InSession, &GraspReadyActionStateInfo, &GraspReadyActionState));

	XrActionStateGetInfo PinchActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	PinchActionStateInfo.action = PinchAction;
	XR_ENSURE(xrGetActionStateFloat(InSession, &PinchActionStateInfo, &PinchValueActionState));

	XrActionStateGetInfo GraspActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };
	GraspActionStateInfo.action = GraspAction;
	XR_ENSURE(xrGetActionStateFloat(InSession, &GraspActionStateInfo, &GraspValueActionState));
}

void FVarjoHandInteraction::FVarjoInteractionController::CheckAndAddEnhancedInputAction(FEnhancedActionKeyMapping EnhancedActionKeyMapping)
{
	if (EnhancedActionKeyMapping.Key == AimKey)
	{
		AimInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
	if (EnhancedActionKeyMapping.Key == PinchReadyKey)
	{
		PinchReadyInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
	if (EnhancedActionKeyMapping.Key == AimReadyKey)
	{
		AimReadyInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
	if (EnhancedActionKeyMapping.Key == GraspReadyKey)
	{
		GraspReadyInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
	if (EnhancedActionKeyMapping.Key == PinchKey)
	{
		PinchInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
	if (EnhancedActionKeyMapping.Key == GraspKey)
	{
		GraspInputActions.Emplace(EnhancedActionKeyMapping.Action);
	}
}

FVarjoHandInteraction::FVarjoHandInteraction(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) : MessageHandler(InMessageHandler)
{
	IModularFeatures::Get().RegisterModularFeature(IMotionController::GetModularFeatureName(), static_cast<IMotionController*>(this));
	RegisterOpenXRExtensionModularFeature();

	m_Instance = this;
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("FVarjoHandInteraction() register extension feature HandInteraction %p."), m_Instance);
}

FVarjoHandInteraction::~FVarjoHandInteraction()
{
	IModularFeatures::Get().UnregisterModularFeature(IMotionController::GetModularFeatureName(), static_cast<IMotionController*>(this));
	UnregisterOpenXRExtensionModularFeature();
	bActionsAttached = false;
}

bool FVarjoHandInteraction::GetRequiredExtensions(TArray<const ANSICHAR*>& OutExtensions)
{
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("GetRequiredExtensions() XR_EXT_hand_interaction, XR_EXT_palm_pose"));
	OutExtensions.Add("XR_EXT_hand_interaction");
	OutExtensions.Add("XR_EXT_palm_pose");
	return true;
}

void FVarjoHandInteraction::AttachActionSets(TSet<XrActionSet>& OutActionSets)
{
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("AttachActionSets()"));

	check(Instance != XR_NULL_HANDLE);
	LeftInteractionController.AddTrackedDevices(OpenXRHMD);
	RightInteractionController.AddTrackedDevices(OpenXRHMD);
	OutActionSets.Add(HandInteractionActionSet);

	PRAGMA_DISABLE_DEPRECATION_WARNINGS

	const UEnhancedInputDeveloperSettings* InputSettings = GetDefault<UEnhancedInputDeveloperSettings>();
	if (InputSettings)
	{
		for (const auto& Context : InputSettings->DefaultMappingContexts)
		{
			if (Context.InputMappingContext)
			{
				TStrongObjectPtr<const UInputMappingContext> Obj(Context.InputMappingContext.LoadSynchronous());
				InputMappingContextToPriorityMap.Add(Obj, Context.Priority);
			}
			else
			{
				UE_LOG(LogHMD, Warning, TEXT("Default Mapping Contexts contains an Input Mapping Context set to \"None\", ignoring while building HandInteraction actions."));
			}
		}
	}

	if (!InputMappingContextToPriorityMap.IsEmpty())
	{
		for (const auto& MappingContext : InputMappingContextToPriorityMap)
		{
			for (const FEnhancedActionKeyMapping& Mapping : MappingContext.Key->GetMappings())
			{
				LeftInteractionController.CheckAndAddEnhancedInputAction(Mapping);
				RightInteractionController.CheckAndAddEnhancedInputAction(Mapping);
			}
		}
	}
	else
	{
		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Aim_Value.GetFName(), &LeftInteractionController.AimValueActionState);
		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Aim_Value.GetFName(), &RightInteractionController.AimValueActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Pinch_Ready.GetFName(), &LeftInteractionController.PinchReadyActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Pinch_Ready.GetFName(), &RightInteractionController.PinchReadyActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Aim_Ready.GetFName(), &LeftInteractionController.AimReadyActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Aim_Ready.GetFName(), &RightInteractionController.AimReadyActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Grasp_Ready.GetFName(), &LeftInteractionController.GraspReadyActionState);
		KeyReadyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Grasp_Ready.GetFName(), &RightInteractionController.GraspReadyActionState);

		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Pinch_Value.GetFName(), &LeftInteractionController.PinchValueActionState);
		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Pinch_Value.GetFName(), &RightInteractionController.PinchValueActionState);
		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Left_Grasp_Value.GetFName(), &LeftInteractionController.GraspValueActionState);
		KeyActionStates.Emplace(HandInteractionKeys::HandInteraction_Right_Grasp_Value.GetFName(), &RightInteractionController.GraspValueActionState);
	}

	PRAGMA_ENABLE_DEPRECATION_WARNINGS

	bActionsAttached = true;
}

void FVarjoHandInteraction::GetActiveActionSetsForSync(TArray<XrActiveActionSet>& OutActiveSets)
{
	check(HandInteractionActionSet != XR_NULL_HANDLE);
	OutActiveSets.Add(XrActiveActionSet{ HandInteractionActionSet, XR_NULL_PATH });
}

void FVarjoHandInteraction::PostCreateInstance(XrInstance InInstance)
{
	Instance = InInstance;
}

const void* FVarjoHandInteraction::OnCreateSession(XrInstance InInstance, XrSystemId InSystem, const void* InNext)
{
	Instance = InInstance;

	static FName SystemName(TEXT("OpenXR"));
	if (GEngine->XRSystem.IsValid() && (GEngine->XRSystem->GetSystemName() == SystemName))
	{
		OpenXRHMD = (FOpenXRHMD*)GEngine->XRSystem.Get();
	}

	bSessionStarted = true;

	// Setup FKeys
	LeftInteractionController.AimKey = HandInteractionKeys::HandInteraction_Left_Aim_Value;
	RightInteractionController.AimKey = HandInteractionKeys::HandInteraction_Right_Aim_Value;
	LeftInteractionController.PinchReadyKey = HandInteractionKeys::HandInteraction_Left_Pinch_Ready;
	RightInteractionController.PinchReadyKey = HandInteractionKeys::HandInteraction_Right_Pinch_Ready;
	LeftInteractionController.AimReadyKey = HandInteractionKeys::HandInteraction_Left_Aim_Ready;
	RightInteractionController.AimReadyKey = HandInteractionKeys::HandInteraction_Right_Aim_Ready;
	LeftInteractionController.GraspReadyKey = HandInteractionKeys::HandInteraction_Left_Grasp_Ready;
	RightInteractionController.GraspReadyKey = HandInteractionKeys::HandInteraction_Right_Grasp_Ready;

	LeftInteractionController.PinchKey = HandInteractionKeys::HandInteraction_Left_Pinch_Value;
	RightInteractionController.PinchKey = HandInteractionKeys::HandInteraction_Right_Pinch_Value;
	LeftInteractionController.GraspKey = HandInteractionKeys::HandInteraction_Left_Grasp_Value;
	RightInteractionController.GraspKey = HandInteractionKeys::HandInteraction_Right_Grasp_Value;

	// Reset actions
	LeftInteractionController.AimPoseAction = XR_NULL_HANDLE;
	LeftInteractionController.GripPoseAction = XR_NULL_HANDLE;
	LeftInteractionController.PinchPoseAction = XR_NULL_HANDLE;
	LeftInteractionController.PokePoseAction = XR_NULL_HANDLE;
	LeftInteractionController.PalmPoseAction = XR_NULL_HANDLE;

	LeftInteractionController.AimAction = XR_NULL_HANDLE;
	LeftInteractionController.GraspAction = XR_NULL_HANDLE;
	LeftInteractionController.PinchAction = XR_NULL_HANDLE;

	LeftInteractionController.AimReadyAction = XR_NULL_HANDLE;
	LeftInteractionController.GraspReadyAction = XR_NULL_HANDLE;
	LeftInteractionController.PinchReadyAction = XR_NULL_HANDLE;

	RightInteractionController.AimPoseAction = XR_NULL_HANDLE;
	RightInteractionController.GripPoseAction = XR_NULL_HANDLE;
	RightInteractionController.PinchPoseAction = XR_NULL_HANDLE;
	RightInteractionController.PokePoseAction = XR_NULL_HANDLE;
	RightInteractionController.PalmPoseAction = XR_NULL_HANDLE;

	RightInteractionController.AimAction = XR_NULL_HANDLE;
	RightInteractionController.GraspAction = XR_NULL_HANDLE;
	RightInteractionController.PinchAction = XR_NULL_HANDLE;

	RightInteractionController.AimReadyAction = XR_NULL_HANDLE;
	RightInteractionController.GraspReadyAction = XR_NULL_HANDLE;
	RightInteractionController.PinchReadyAction = XR_NULL_HANDLE;

	// Setup XrPaths for interaction controllers
	LeftInteractionController.SetupPath(HandInteractionRolePath::Left, HandInteractionActionPath::LeftAimPose, HandInteractionActionPath::LeftGripPose, HandInteractionActionPath::LeftPinchPose, HandInteractionActionPath::LeftPokePose, HandInteractionActionPath::LeftPalmPose, HandInteractionActionPath::LeftAimValue, HandInteractionActionPath::LeftGraspValue, HandInteractionActionPath::LeftPinchValue, HandInteractionActionPath::LeftAimReady, HandInteractionActionPath::LeftGraspReady, HandInteractionActionPath::LeftPinchReady);
	RightInteractionController.SetupPath(HandInteractionRolePath::Right, HandInteractionActionPath::RightAimPose, HandInteractionActionPath::RightGripPose, HandInteractionActionPath::RightPinchPose, HandInteractionActionPath::RightPokePose, HandInteractionActionPath::RightPalmPose, HandInteractionActionPath::RightAimValue, HandInteractionActionPath::RightGraspValue, HandInteractionActionPath::RightPinchValue, HandInteractionActionPath::RightAimReady, HandInteractionActionPath::RightGraspReady, HandInteractionActionPath::RightPinchReady);

	if (HandInteractionActionSet != XR_NULL_HANDLE)
	{
		xrDestroyActionSet(HandInteractionActionSet);
		HandInteractionActionSet = XR_NULL_HANDLE;
	}

	// ActionSet and bindings
	XrActionSetCreateInfo Info;
	Info.type = XR_TYPE_ACTION_SET_CREATE_INFO;
	Info.next = nullptr;
	FCStringAnsi::Strcpy(Info.actionSetName, XR_MAX_ACTION_SET_NAME_SIZE, "varjohandinteractionactionset");
	FCStringAnsi::Strcpy(Info.localizedActionSetName, XR_MAX_LOCALIZED_ACTION_SET_NAME_SIZE, "Varjo Hand Interaction Action Set");
	Info.priority = 0;
	XR_ENSURE(xrCreateActionSet(Instance, &Info, &HandInteractionActionSet));

	LeftInteractionController.AddActions(HandInteractionActionSet);
	RightInteractionController.AddActions(HandInteractionActionSet);

	UE_LOG(LogVarjoHandInteraction, Log, TEXT("xrSuggestInteractionProfileBindings()"));
	TArray<XrActionSuggestedBinding> Bindings;
	LeftInteractionController.GetSuggestedBindings(Bindings);
	RightInteractionController.GetSuggestedBindings(Bindings);
	XrInteractionProfileSuggestedBinding InteractionProfileSuggestedBindings{ XR_TYPE_INTERACTION_PROFILE_SUGGESTED_BINDING };

	InteractionProfileSuggestedBindings.interactionProfile = FOpenXRPath("/interaction_profiles/ext/hand_interaction_ext");

	InteractionProfileSuggestedBindings.suggestedBindings = Bindings.GetData();
	InteractionProfileSuggestedBindings.countSuggestedBindings = Bindings.Num();
	XR_ENSURE(xrSuggestInteractionProfileBindings(Instance, &InteractionProfileSuggestedBindings));

	return InNext;
}

void FVarjoHandInteraction::OnDestroySession(XrSession InSession)
{
	if (bActionsAttached)
	{
		bActionsAttached = false;
		bSessionStarted = false;
		KeyActionStates.Reset();
		KeyReadyActionStates.Reset();
	}
}

const void* FVarjoHandInteraction::OnBeginSession(XrSession InSession, const void* InNext)
{
	bSessionStarted = true;

	return InNext;
}

void FVarjoHandInteraction::PostSyncActions(XrSession InSession)
{
	WorldToMetersScale_ = OpenXRHMD->GetWorldToMetersScale();

	LeftInteractionController.SyncActionStates(InSession);
	RightInteractionController.SyncActionStates(InSession);

	SendInputEvent_EnhancedInput();
}

#pragma region
bool FVarjoHandInteraction::GetControllerOrientationAndPosition(const int32 ControllerIndex, const FName MotionSource, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
	if (!bActionsAttached || OpenXRHMD == nullptr)
	{
		return false;
	}

	if (ControllerIndex == DeviceIndex)
	{
		if (MotionSource == HandInteractionMotionSource::LeftPinch || MotionSource == HandInteractionMotionSource::RightPinch)
		{
			int32 DeviceId = LeftInteractionController.PinchDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPinch) DeviceId = RightInteractionController.PinchDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetCurrentPose(DeviceId, Orientation, OutPosition);

			OutOrientation = FRotator(Orientation);
			return Success;
		}

		if (MotionSource == HandInteractionMotionSource::LeftPoke || MotionSource == HandInteractionMotionSource::RightPoke)
		{
			int32 DeviceId = LeftInteractionController.PokeDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPoke) DeviceId = RightInteractionController.PokeDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetCurrentPose(DeviceId, Orientation, OutPosition);

			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftPalm || MotionSource == HandInteractionMotionSource::RightPalm)
		{
			int32 DeviceId = LeftInteractionController.PalmDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPalm) DeviceId = RightInteractionController.PalmDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetCurrentPose(DeviceId, Orientation, OutPosition);

			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftAim || MotionSource == HandInteractionMotionSource::RightAim)
		{
			int32 AimDeviceId = LeftInteractionController.AimDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightAim) AimDeviceId = RightInteractionController.AimDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetCurrentPose(AimDeviceId, Orientation, OutPosition);

			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftGrip || MotionSource == HandInteractionMotionSource::RightGrip)
		{
			int32 GripDeviceId = LeftInteractionController.GripDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightGrip) GripDeviceId = RightInteractionController.GripDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetCurrentPose(GripDeviceId, Orientation, OutPosition);

			OutOrientation = FRotator(Orientation);
			return Success;
		}
	}
	return false;
}

bool FVarjoHandInteraction::GetControllerOrientationAndPosition(const int32 ControllerIndex, const FName MotionSource, FRotator& OutOrientation, FVector& OutPosition, bool& OutbProvidedLinearVelocity, FVector& OutLinearVelocity, bool& OutbProvidedAngularVelocity, FVector& OutAngularVelocityAsAxisAndLength, bool& OutbProvidedLinearAcceleration, FVector& OutLinearAcceleration, float WorldToMetersScale) const
{
	FTimespan Time;
	bool OutTimeWasUsed = false;
	return GetControllerOrientationAndPositionForTime(ControllerIndex, MotionSource, Time, OutTimeWasUsed, OutOrientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityAsAxisAndLength, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
}

bool FVarjoHandInteraction::GetControllerOrientationAndPositionForTime(const int32 ControllerIndex, const FName MotionSource, FTimespan Time, bool& OutTimeWasUsed, FRotator& OutOrientation, FVector& OutPosition, bool& OutbProvidedLinearVelocity, FVector& OutLinearVelocity, bool& OutbProvidedAngularVelocity, FVector& OutAngularVelocityRadPerSec, bool& OutbProvidedLinearAcceleration, FVector& OutLinearAcceleration, float WorldToMetersScale) const
{
	if (!bActionsAttached || OpenXRHMD == nullptr)
	{
		return false;
	}

	if (ControllerIndex == DeviceIndex)
	{
		if (MotionSource == HandInteractionMotionSource::LeftPinch || MotionSource == HandInteractionMotionSource::RightPinch)
		{
			int32 DeviceId = LeftInteractionController.PinchDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPinch) DeviceId = RightInteractionController.PinchDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetPoseForTime(DeviceId, Time, OutTimeWasUsed, Orientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityRadPerSec, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftPoke || MotionSource == HandInteractionMotionSource::RightPoke)
		{
			int32 DeviceId = LeftInteractionController.PokeDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPoke) DeviceId = RightInteractionController.PokeDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetPoseForTime(DeviceId, Time, OutTimeWasUsed, Orientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityRadPerSec, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftPalm || MotionSource == HandInteractionMotionSource::RightPalm)
		{
			int32 DeviceId = LeftInteractionController.PalmDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightPalm) DeviceId = RightInteractionController.PalmDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetPoseForTime(DeviceId, Time, OutTimeWasUsed, Orientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityRadPerSec, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftAim || MotionSource == HandInteractionMotionSource::RightAim)
		{
			int32 DeviceId = LeftInteractionController.AimDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightAim) DeviceId = RightInteractionController.AimDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetPoseForTime(DeviceId, Time, OutTimeWasUsed, Orientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityRadPerSec, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
			OutOrientation = FRotator(Orientation);
			return Success;
		}
		if (MotionSource == HandInteractionMotionSource::LeftGrip || MotionSource == HandInteractionMotionSource::RightGrip)
		{
			int32 DeviceId = LeftInteractionController.GripDeviceId;
			if (MotionSource == HandInteractionMotionSource::RightGrip) DeviceId = RightInteractionController.GripDeviceId;

			FQuat Orientation;
			bool Success = OpenXRHMD->GetPoseForTime(DeviceId, Time, OutTimeWasUsed, Orientation, OutPosition, OutbProvidedLinearVelocity, OutLinearVelocity, OutbProvidedAngularVelocity, OutAngularVelocityRadPerSec, OutbProvidedLinearAcceleration, OutLinearAcceleration, WorldToMetersScale);
			OutOrientation = FRotator(Orientation);
			return Success;
		}
	}
	return false;

}

ETrackingStatus FVarjoHandInteraction::GetControllerTrackingStatus(const int32 ControllerIndex, const FName MotionSource) const
{
	if (!bActionsAttached || OpenXRHMD == nullptr)
	{
		return ETrackingStatus::NotTracked;
	}

	XrSession Session = OpenXRHMD->GetSession();
	if (Session == XR_NULL_HANDLE)
	{
		return ETrackingStatus::NotTracked;
	}

	XrActionStateGetInfo PoseActionStateInfo{ XR_TYPE_ACTION_STATE_GET_INFO };

	if (MotionSource == HandInteractionMotionSource::LeftPinch)
	{
		PoseActionStateInfo.action = LeftInteractionController.PinchPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::RightPinch)
	{
		PoseActionStateInfo.action = RightInteractionController.PinchPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::LeftPoke)
	{
		PoseActionStateInfo.action = LeftInteractionController.PokePoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::RightPoke)
	{
		PoseActionStateInfo.action = RightInteractionController.PokePoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::LeftPalm)
	{
		PoseActionStateInfo.action = LeftInteractionController.PalmPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::RightPalm)
	{
		PoseActionStateInfo.action = RightInteractionController.PalmPoseAction;
	}

	if (MotionSource == HandInteractionMotionSource::LeftAim)
	{
		PoseActionStateInfo.action = LeftInteractionController.AimPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::RightAim)
	{
		PoseActionStateInfo.action = RightInteractionController.AimPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::LeftGrip)
	{
		PoseActionStateInfo.action = LeftInteractionController.GripPoseAction;
	}
	else if (MotionSource == HandInteractionMotionSource::RightGrip)
	{
		PoseActionStateInfo.action = RightInteractionController.GripPoseAction;
	}
	else
	{
		return ETrackingStatus::NotTracked;
	}
	PoseActionStateInfo.subactionPath = XR_NULL_PATH;

	XrActionStatePose State = { XR_TYPE_ACTION_STATE_POSE };
	if (!XR_ENSURE(xrGetActionStatePose(Session, &PoseActionStateInfo, &State)))
	{
		return ETrackingStatus::NotTracked;
	}

	return State.isActive ? ETrackingStatus::Tracked : ETrackingStatus::NotTracked;
}

FName FVarjoHandInteraction::GetMotionControllerDeviceTypeName() const
{
	const static FName DefaultName(TEXT("OpenXRVarjoHandInteraction"));
	return DefaultName;
}

void FVarjoHandInteraction::EnumerateSources(TArray<FMotionControllerSource>& SourcesOut) const
{
	check(IsInGameThread());

	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::LeftAim));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::RightAim));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::LeftGrip));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::RightGrip));

	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::LeftPinch));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::RightPinch));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::LeftPoke));
	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::RightPoke));

// FIXME: this can be added back after the extension is implemented in OpenXR runtime
//	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::LeftPalm));
//	SourcesOut.Add(FMotionControllerSource(HandInteractionMotionSource::RightPalm));
}

PRAGMA_DISABLE_DEPRECATION_WARNINGS
bool FVarjoHandInteraction::SetPlayerMappableInputConfig(TObjectPtr<class UPlayerMappableInputConfig> InputConfig)
{
	if (bActionsAttached)
	{
		UE_LOG(LogHMD, Error, TEXT("Attempted to attach an input config while one is already attached for the current session."));

		return false;
	}

	TSet<TObjectPtr<UInputMappingContext>> MappingContexts;
	InputConfig->GetMappingContexts().GetKeys(MappingContexts);
	return AttachInputMappingContexts(MappingContexts);
}

bool FVarjoHandInteraction::AttachInputMappingContexts(const TSet<TObjectPtr<UInputMappingContext>>& MappingContexts)
{
	if (bActionsAttached)
	{
		UE_LOG(LogHMD, Error, TEXT("Attempted to attach input mapping contexts when action sets are already attached for the current session."));

		return false;
	}

	for (const auto& Context : MappingContexts)
	{
		InputMappingContextToPriorityMap.Add(TStrongObjectPtr<UInputMappingContext>(Context), 0);
	}
	return true;
}
PRAGMA_ENABLE_DEPRECATION_WARNINGS

void FVarjoHandInteraction::SendInputEvent_EnhancedInput()
{
	auto InjectEnhancedFloatInput = [](XrActionStateFloat& State, TArray<TObjectPtr<const UInputAction>>& Actions)
	{
		FInputActionValue InputValue;
		TArray<TObjectPtr<UInputTrigger>> Triggers = {};
		TArray<TObjectPtr<UInputModifier>> Modifiers = {};

		InputValue = FInputActionValue(State.isActive ? State.currentState : false);

		for (auto InputAction : Actions)
		{
			auto InjectSubsystemInput = [InputAction, InputValue, Triggers, Modifiers](IEnhancedInputSubsystemInterface* Subsystem)
			{
				if (Subsystem)
				{
					Subsystem->InjectInputForAction(InputAction, InputValue, Modifiers, Triggers);
				}
			};
			IEnhancedInputModule::Get().GetLibrary()->ForEachSubsystem(InjectSubsystemInput);

#if WITH_EDITOR
			if (GEditor)
			{
				InjectSubsystemInput(GEditor->GetEditorSubsystem<UEnhancedInputEditorSubsystem>());
			}
#endif
		}
	};
	auto InjectEnhancedBooleanInput = [](XrActionStateBoolean& State, TArray<TObjectPtr<const UInputAction>>& Actions)
	{
		FInputActionValue InputValue;
		TArray<TObjectPtr<UInputTrigger>> Triggers = {};
		TArray<TObjectPtr<UInputModifier>> Modifiers = {};

		InputValue = FInputActionValue(State.isActive ? (bool)State.currentState : false);

		for (auto InputAction : Actions)
		{
			auto InjectSubsystemInput = [InputAction, InputValue, Triggers, Modifiers](IEnhancedInputSubsystemInterface* Subsystem)
			{
				if (Subsystem)
				{
					Subsystem->InjectInputForAction(InputAction, InputValue, Modifiers, Triggers);
				}
			};

			IEnhancedInputModule::Get().GetLibrary()->ForEachSubsystem(InjectSubsystemInput);

#if WITH_EDITOR
			if (GEditor)
			{
				InjectSubsystemInput(GEditor->GetEditorSubsystem<UEnhancedInputEditorSubsystem>());
			}
#endif
		}
	};

	InjectEnhancedFloatInput(LeftInteractionController.AimValueActionState, LeftInteractionController.AimInputActions);
	InjectEnhancedFloatInput(RightInteractionController.AimValueActionState, RightInteractionController.AimInputActions);
	InjectEnhancedBooleanInput(LeftInteractionController.PinchReadyActionState, LeftInteractionController.PinchReadyInputActions);
	InjectEnhancedBooleanInput(RightInteractionController.PinchReadyActionState, RightInteractionController.PinchReadyInputActions);
	InjectEnhancedBooleanInput(LeftInteractionController.AimReadyActionState, LeftInteractionController.AimReadyInputActions);
	InjectEnhancedBooleanInput(RightInteractionController.AimReadyActionState, RightInteractionController.AimReadyInputActions);
	InjectEnhancedBooleanInput(LeftInteractionController.GraspReadyActionState, LeftInteractionController.GraspReadyInputActions);
	InjectEnhancedBooleanInput(RightInteractionController.GraspReadyActionState, RightInteractionController.GraspReadyInputActions);
	InjectEnhancedFloatInput(LeftInteractionController.PinchValueActionState, LeftInteractionController.PinchInputActions);
	InjectEnhancedFloatInput(RightInteractionController.PinchValueActionState, RightInteractionController.PinchInputActions);
	InjectEnhancedFloatInput(LeftInteractionController.GraspValueActionState, LeftInteractionController.GraspInputActions);
	InjectEnhancedFloatInput(RightInteractionController.GraspValueActionState, RightInteractionController.GraspInputActions);
}

#pragma endregion IInputDevice overrides

FVarjoHandInteractionModule::FVarjoHandInteractionModule() 
{
}

FName IVarjoHandInteractionModule::VarjoHandInteractionModularKeyName = FName(TEXT("VarjoHandInteraction"));

void FVarjoHandInteractionModule::StartupModule()
{
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("StartupModule() Entry."));

	TSharedPtr<FGenericApplicationMessageHandler> DummyMessageHandler(new FGenericApplicationMessageHandler());
	CreateInputDevice(DummyMessageHandler.ToSharedRef());

	EKeys::AddMenuCategoryDisplayInfo("Hand Interaction", LOCTEXT("HandInteractionSubCategory", "Varjo Hand Interaction"), TEXT("GraphEditor.PadEvent_16x"));

	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Aim_Value, LOCTEXT("HandInteraction_Left_Aim_Value", "Hand Interaction (L) Aim Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Pinch_Ready, LOCTEXT("HandInteraction_Left_Pinch_Ready", "Hand Interaction (L) Pinch Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Aim_Ready, LOCTEXT("HandInteraction_Left_Aim_Ready", "Hand Interaction (L) Aim Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Grasp_Ready, LOCTEXT("HandInteraction_Left_Grasp_Ready", "Hand Interaction (L) Grasp Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));

	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Pinch_Value, LOCTEXT("HandInteraction_Left_Pinch_Value", "Hand Interaction (L) Pinch Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Left_Grasp_Value, LOCTEXT("HandInteraction_Left_Grasp_Value", "Hand Interaction (L) Grasp Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));

	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Aim_Value, LOCTEXT("HandInteraction_Right_Aim_Value", "Hand Interaction (R) Aim Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Pinch_Ready, LOCTEXT("HandInteraction_Right_Pinch_Ready", "Hand Interaction (R) Pinch Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Aim_Ready, LOCTEXT("HandInteraction_Right_Aim_Ready", "Hand Interaction (R) Aim Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Grasp_Ready, LOCTEXT("HandInteraction_Right_Grasp_Ready", "Hand Interaction (R) Grasp Ready"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));

	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Pinch_Value, LOCTEXT("HandInteraction_Right_Pinch_Value", "Hand Interaction (R) Pinch Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));
	EKeys::AddKey(FKeyDetails(HandInteractionKeys::HandInteraction_Right_Grasp_Value, LOCTEXT("HandInteraction_Right_Grasp_Value", "Hand Interaction (R) Grasp Value"), FKeyDetails::GamepadKey | FKeyDetails::NotBlueprintBindableKey, "Hand Interaction"));

	UE_LOG(LogVarjoHandInteraction, Log, TEXT("StartupModule() Finished."));
}

void FVarjoHandInteractionModule::ShutdownModule()
{
	UE_LOG(LogVarjoHandInteraction, Log, TEXT("ShutdownModule()"));
}

TSharedPtr<class IInputDevice> FVarjoHandInteractionModule::CreateInputDevice(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	if (!HandInteractionController.IsValid())
	{
		auto InputDevice = new FVarjoHandInteraction(InMessageHandler);
		HandInteractionController = TSharedPtr<FVarjoHandInteraction>(InputDevice);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("CreateInputDevice() new HandInteractionController %p"), HandInteractionController.Get());

		return HandInteractionController;
	}
	else
	{
		HandInteractionController.Get()->SetMessageHandler(InMessageHandler);
		UE_LOG(LogVarjoHandInteraction, Log, TEXT("CreateInputDevice() update HandInteractionController %p"), HandInteractionController.Get());

		return HandInteractionController;
	}

	return nullptr;
}

FVarjoHandInteraction* FVarjoHandInteractionModule::GetHandInteraction()
{
	return FVarjoHandInteraction::GetInstance();
}

IMPLEMENT_MODULE(FVarjoHandInteractionModule, VarjoHandInteraction);

#undef LOCTEXT_NAMESPACE
