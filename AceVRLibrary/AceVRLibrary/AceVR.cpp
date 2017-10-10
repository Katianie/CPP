#pragma once
#include "AceVR.h"

/**
* Converts SteamVR matrix to DirectX Matrix (XMMATRIX).
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		HmdMatrix34_t steamVrMatrix;
*		steamVrMatrix.m[0][0] = 1.0; steamVrMatrix.m[0][1] = 2.0;  steamVrMatrix.m[0][2] = 3.0;	 steamVrMatrix.m[0][3] = 4.0;
*		steamVrMatrix.m[1][0] = 5.0; steamVrMatrix.m[1][1] = 6.0;  steamVrMatrix.m[1][2] = 7.0;	 steamVrMatrix.m[1][3] = 8.0;
*		steamVrMatrix.m[2][0] = 9.0; steamVrMatrix.m[2][1] = 10.0; steamVrMatrix.m[2][2] = 11.0; steamVrMatrix.m[2][3] = 12.0;
*
*		XMMATRIX dxMatrix = AceVR::ConvertSteamVRMatrixToXMMATRIX(steamVrMatrix);
*		char* dxMatrixToString = AceVR::XMMatrixToString(dxMatrix);
*		printf("%s\n", dxMatrixToString);
*		_aligned_free(dxMatrixToString);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output:[1.000000,	2.000000,	3.000000,	4.000000]
*		 [5.000000,	6.000000,	7.000000,	8.000000]
*		 [9.000000,	10.000000,	11.000000,	12.000000]
*		 [0.000000,	0.000000,	0.000000,	1.000000]
*
* @param matPose The SteamVR matrix to convert to XMMATRIX.
*
* @return The converted matrix.
*/
XMMATRIX AceVR::ConvertSteamVRMatrixToXMMATRIX(const HmdMatrix34_t& matPose)
{
	XMMATRIX matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f);

	return XMMatrixTranspose(matrixObj);
}

/**
* Returns true if the VR device has a proximity sensor, false otherwise.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool hasProximitySensor = AceVR::GetContainsProximitySensor(hmd);
*		if (hasProximitySensor == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: true
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the VR device has a proximity sensor, false otherwise.
*/
bool AceVR::GetContainsProximitySensor(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool hasProximitySensor = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_ContainsProximitySensor_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		hasProximitySensor = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetContainsProximitySensor() Parameters are not in valid range.";
	}

	return hasProximitySensor;
}

/**
* Returns true if the specified button is pressed on the VR controller.
*
* @author Katianie
* @date 11/23/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isButtonPressed = AceVR::GetControllerIsButtonPressed(hmd, EVRButtonId::k_EButton_SteamVR_Trigger);
*		if (isButtonPressed == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: true
*
* @param [in] hmd The HMD being used.
* @param button The button on the controller to check.
*
* @return true if the specified button is pressed on the VR controller.
*/
bool AceVR::GetControllerIsButtonPressed(IVRSystem* hmd, EVRButtonId button)
{
	uint64_t buttonMask;
	VRControllerState_t state;

	if (hmd != NULL)
	{
		for (TrackedDeviceIndex_t unDevice = 0; unDevice < k_unMaxTrackedDeviceCount; unDevice++)
		{
			if (hmd->GetControllerState(unDevice, &state) == true)
			{
				buttonMask = ButtonMaskFromId(button);
				if ((state.ulButtonPressed & buttonMask) != 0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

/**
* Returns true if the specified button is touched on the VR controller.
*
* @author Katianie
* @date 11/23/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isButtonTouched = AceVR::GetControllerIsButtonTouched(hmd, EVRButtonId::k_EButton_SteamVR_Touchpad);
*		if (isButtonTouched == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: true
*
* @param [in] hmd The HMD being used.
* @param button The button on the controller to check.
*
* @return true if the specified button is touched on the VR controller.
*/
bool AceVR::GetControllerIsButtonTouched(IVRSystem* hmd, EVRButtonId button)
{
	uint64_t buttonMask;
	VRControllerState_t state;

	if (hmd != NULL)
	{
		for (TrackedDeviceIndex_t unDevice = 0; unDevice < k_unMaxTrackedDeviceCount; unDevice++)
		{
			if (hmd->GetControllerState(unDevice, &state) == true)
			{
				buttonMask = ButtonMaskFromId(button);
				if ((state.ulButtonTouched & buttonMask) != 0)
				{
					return true;
				}
			}
		}
	}

	return false;
}

/**
* Gets current view projection matrix.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		XMMATRIX eyeMatrix = AceVR::GetHMDEyePoseMatrix(hmd, Eye_Right);
*		XMMATRIX projMatrix = AceVR::GetHMDEyeProjectionMatrix(hmd, Eye_Right, 0.01f, 1000.0f);
*		XMMATRIX dxMVPmatrix = AceVR::GetCurrentViewProjectionMatrix(hmd, projMatrix, eyeMatrix, XMMATRIX());
*		char* dxMatrixToString = AceVR::XMMatrixToString(dxMVPmatrix);
*		printf("%s\n", dxMatrixToString);
*		_aligned_free(dxMatrixToString);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output:[1.000000,	2.000000,	3.000000,	4.000000]
*		 [5.000000,	6.000000,	7.000000,	8.000000]
*		 [9.000000,	10.000000,	11.000000,	12.000000]
*		 [0.000000,	0.000000,	0.000000,	1.000000]
*
* @param [in] hmd The HMD being used.
* @param projectionMat The Projection Matrix.
* @param eyePose The Eye Pose Matrix.
* @param hmdPose The HMD Pose.
*
* @return The current view projection matrix.
*/
XMMATRIX AceVR::GetCurrentViewProjectionMatrix(IVRSystem* hmd, const XMMATRIX& projectionMat, const XMMATRIX& eyePose, const XMMATRIX& hmdPose)
{
	return projectionMat * eyePose * hmdPose;
}

/**
* Gets the battery's charge in percentage of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		float percent = AceVR::GetDeviceBatteryPercentage(hmd);
*		printf("%f\n", percent);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0.000000
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The percentage of battery life (1 being fully charged, 0 dead).
*/
float AceVR::GetDeviceBatteryPercentage(IVRSystem* hmd, uint32_t deviceIndex)
{
	// 0 is empty, 1 is full
	float percentile = 0.0f;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DeviceBatteryPercentage_Float;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		percentile = hmd->GetFloatTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetDeviceBatteryPercentage() Parameters are not in valid range.";
	}

	return percentile * 100.0f;
}

/**
* Gets the device class of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		int32_t deviceClass = AceVR::GetDeviceClass(hmd);
*		printf("%d\n", deviceClass);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return An integer corresponding to a device class.
*/
uint64_t AceVR::GetDeviceClass(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t deviceClass = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DeviceClass_Int32;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		deviceClass = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetDeviceClass() Parameters are not in valid range.";
	}

	return deviceClass;
}

/**
* Checks if the device is charging.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isCharging = AceVR::GetDeviceIsCharging(hmd);
*		if (isCharging == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: false
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the device is charging, false otherwise.
*/
bool AceVR::GetDeviceIsCharging(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool isCharging = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DeviceIsCharging_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		isCharging = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetDeviceIsCharging() Parameters are not in valid range.";
	}

	return isCharging;
}

/**
* Checks if the device is wireless or not.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isWireless = AceVR::GetDeviceIsWireless(hmd);
*		if (isWireless == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: false
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the VR device is wireless, false otherwise.
*/
bool AceVR::GetDeviceIsWireless(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool isWireless = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DeviceIsWireless_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		isWireless = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetDeviceIsWireless() Parameters are not in valid range.";
	}

	return isWireless;
}

/**
* Returns true if the VR device can provide battery status, false otherwise
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool hasBatteryStatus = AceVR::GetDeviceProvidesBatteryStatus(hmd);
*		if (hasBatteryStatus == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: false
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the VR device can provide battery status, false otherwise.
*/
bool AceVR::GetDeviceProvidesBatteryStatus(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool hasBatteryStatus = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DeviceProvidesBatteryStatus_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		hasBatteryStatus = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetDeviceProvidesBatteryStatus() Parameters are not in valid range.";
	}

	return hasBatteryStatus;
}

/**
* Gets the Firmware Programming Target of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* firmwareProgrammingTarg = AceVR::GetFirmwareProgrammingTarget(hmd);
*		printf("%s\n", firmwareProgrammingTarg);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: LHR-F18C3E71
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the firmware programming target.
*/
char* AceVR::GetFirmwareProgrammingTarget(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_Firmware_ProgrammingTarget_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetFirmwareProgrammingTarget() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetFirmwareProgrammingTarget() Parameters are not in valid range.";
	}

	return buffer;
}

/**
* Gets the Firmware Version of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t revNum = AceVR::GetFirmwareVersion(hmd);
*		printf("%d\n", revNum);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 1462663157
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return An unsigned integer indicating the firmware version.
*/
uint64_t AceVR::GetFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t firmwareVersion = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_FirmwareVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		firmwareVersion = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetFirmwareVersion() Parameters are not in valid range.";
	}

	return firmwareVersion;
}

/**
* Gets the FPGA Version of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t fpgaVer = AceVR::GetFPGAVersion(hmd);
*		printf("%d\n", fpgaVer);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 262
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return An unsigned integer indicating the FPGA version.
*/
uint64_t AceVR::GetFPGAVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t fpgaVersion = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_FPGAVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		fpgaVersion = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetFPGAVersion() Parameters are not in valid range.";
	}

	return fpgaVersion;
}

/**
* Returns true of the HMD has a camera, false otherwise.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool hasCamera = AceVR::GetHasCamera(hmd);
*		if (hasCamera == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: true
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the HMD. has a camera, false otherwise.
*/
bool AceVR::GetHasCamera(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool hasCamera = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_HasCamera_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		hasCamera = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetHasCamera() Parameters are not in valid range.";
	}

	return hasCamera;
}

/**
* Gets the Audio Firmware Version of the HMD. Property is unique
* to TrackedDeviceClass_HMD
*
* @author Katianie
* @date 11/9/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDAudioFirmwareVersion(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 3
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Audio Firmware Version of the HMD.
*/
uint64_t AceVR::GetHMDAudioFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t audioFirmwareVer = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_AudioFirmwareVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			audioFirmwareVer = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDAudioFirmwareVersion() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDAudioFirmwareVersion() Parameters are not in valid range.";
	}

	return audioFirmwareVer;
}

/**
* Gets the Camera Capability Mode of the HMD. Property is unique
* to TrackedDeviceClass_HMD
*
* @author Katianie
* @date 11/9/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		int32_t retVal = AceVR::GetHMDCameraCompatibilityMode(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Camera Capability Mode of the HMD.
*/
uint64_t AceVR::GetHMDCameraCompatibilityMode(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t cameraCompatibilityMode = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_CameraCompatibilityMode_Int32;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			cameraCompatibilityMode = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDCameraCompatibilityMode() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDCameraCompatibilityMode() Parameters are not in valid range.";
	}

	return cameraCompatibilityMode;
}

/**
* Gets the Current Universe ID of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDCurrentUniverseId(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 1469236093
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Current Universe ID of the HMD.
*/
uint64_t AceVR::GetHMDCurrentUniverseId(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t currUniverseID = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_CurrentUniverseId_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			currUniverseID = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDCurrentUniverseId() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDCurrentUniverseId() Parameters are not in valid range.";
	}

	return currUniverseID;
}

/**
* Gets the Display Bootloader Version of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDDisplayBootloaderVersion(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 1048584
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Display Bootloader Version of the HMD.
*/
uint64_t AceVR::GetHMDDisplayBootloaderVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t displayBootLoaderVer = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DisplayBootloaderVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			displayBootLoaderVer = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDDisplayFirmwareVersion() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDDisplayFirmwareVersion() Parameters are not in valid range.";
	}

	return displayBootLoaderVer;
}

/**
* Gets the Display Firmware Version of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDDisplayFirmwareVersion(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 2097432
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Display Firmware Version of the HMD.
*/
uint64_t AceVR::GetHMDDisplayFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t displayFirmwareVer = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DisplayFirmwareVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			displayFirmwareVer = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDDisplayFirmwareVersion() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDDisplayFirmwareVersion() Parameters are not in valid range.";
	}

	return displayFirmwareVer;
}

/**
* Gets the display frequency of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		float retVal = AceVR::GetHMDDisplayFrequency(hmd);
*		printf("%f\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 90.000000
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The display frequency of the HMD.
*/
float AceVR::GetHMDDisplayFrequency(IVRSystem* hmd, uint32_t deviceIndex)
{
	float hmdDisplayFreq = 0.0f;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DisplayFrequency_Float;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			hmdDisplayFreq = hmd->GetFloatTrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDDisplayFrequency() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDDisplayFrequency() Parameters are not in valid range.";
	}

	return hmdDisplayFreq;
}

/**
* Gets the Display Hardware Version of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDDisplayHardwareVersion(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 19
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Display Hardware Version.
*/
uint64_t AceVR::GetHMDDisplayHardwareVersion(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t displayHardwareVer = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_DisplayHardwareVersion_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			displayHardwareVer = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDDisplayHardwareVersion() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDDisplayHardwareVersion() Parameters are not in valid range.";
	}

	return displayHardwareVer;
}

/**
* Gets the HMD Pose (i.e. Orientation) Matrix.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		XMMATRIX dxMatrix = AceVR::GetHMDEyePoseMatrix(hmd, Eye_Right);
*		char* dxMatrixToString = AceVR::XMMatrixToString(dxMatrix);
*		printf("%s\n", dxMatrixToString);
*		_aligned_free(dxMatrixToString);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output:[1.000000,	0.000000,	0.000000,	-0.032000]
*		 [0.000000,	1.000000,	0.000000,	0.000000]
*		 [0.000000,	0.000000,	1.000000,	-0.015000]
*		 [0.000000,	0.000000,	0.000000,	1.000000]
*
* @param [in] hmd The HMD being used.
* @param nEye The left or right eye.
*
* @return The HMD Eye Pose Matrix.
*/
XMMATRIX AceVR::GetHMDEyePoseMatrix(IVRSystem* hmd, Hmd_Eye nEye)
{
	HmdMatrix34_t matEyeHeadTransform;

	if (hmd != NULL)
	{
		matEyeHeadTransform = hmd->GetEyeToHeadTransform(nEye);

		//Transpose and convert the 3x4 matrix in one go.
		XMMATRIX matrixObj(
			matEyeHeadTransform.m[0][0], matEyeHeadTransform.m[1][0], matEyeHeadTransform.m[2][0], 0.0,
			matEyeHeadTransform.m[0][1], matEyeHeadTransform.m[1][1], matEyeHeadTransform.m[2][1], 0.0,
			matEyeHeadTransform.m[0][2], matEyeHeadTransform.m[1][2], matEyeHeadTransform.m[2][2], 0.0,
			matEyeHeadTransform.m[0][3], matEyeHeadTransform.m[1][3], matEyeHeadTransform.m[2][3], 1.0f);

		//Invert the matrix because the formula says so.
		return XMMatrixInverse(NULL, XMMatrixTranspose(matrixObj));
	}

	return XMMATRIX();
}

/**
* Gets The Eye Projection Matrix for the specified eye.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		XMMATRIX dxMatrix = AceVR::GetHMDEyeProjectionMatrix(hmd, Eye_Right, 0.01f, 1000.0f);
*		char* dxMatrixToString = AceVR::XMMatrixToString(dxMatrix);
*		printf("%s\n", dxMatrixToString);
*		_aligned_free(dxMatrixToString);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:[0.757698,	0.000000,	0.057201,	0.000000]
*		 [0.000000,	0.682125,	-0.002325,	0.000000]
*		 [0.000000,	0.000000,	-1.000010,	-0.010000]
*		 [0.000000,	0.000000,	-1.000000,	0.000000]
*
* @param [in] hmd The HMD being used.
* @param nEye The eye.
* @param nearClipPlane The near clip plane.
* @param farClipPlane The far clip plane.
*
* @return The HMD's matrix projection eye.
*/
XMMATRIX AceVR::GetHMDEyeProjectionMatrix(IVRSystem* hmd, Hmd_Eye nEye, float nearClipPlane, float farClipPlane)
{
	HmdMatrix44_t projMat;

	if (hmd != NULL)
	{
		projMat = hmd->GetProjectionMatrix(nEye, nearClipPlane, farClipPlane, API_DirectX);

		return XMMatrixTranspose(XMMATRIX(
			projMat.m[0][0], projMat.m[1][0], projMat.m[2][0], projMat.m[3][0],
			projMat.m[0][1], projMat.m[1][1], projMat.m[2][1], projMat.m[3][1],
			projMat.m[0][2], projMat.m[1][2], projMat.m[2][2], projMat.m[3][2],
			projMat.m[0][3], projMat.m[1][3], projMat.m[2][3], projMat.m[3][3]));
	}

	return XMMATRIX();
}

/**
* Gets the Previous Universe ID of the HMD. Property is unique
* to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		uint64_t retVal = AceVR::GetHMDPreviousUniverseId(hmd);
*		printf("%d\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The Previous Universe ID of the HMD.
*/
uint64_t AceVR::GetHMDPreviousUniverseId(IVRSystem* hmd, uint32_t deviceIndex)
{
	uint64_t prevUniverseID = 0;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_PreviousUniverseId_Uint64;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			prevUniverseID = hmd->GetUint64TrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDPreviousUniverseId() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDPreviousUniverseId() Parameters are not in valid range.";
	}

	return prevUniverseID;
}

/**
* Gets the number of seconds from Vsync to photons of the HMD.
* Property is unique to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		float retVal = AceVR::GetHMDSecondsFromVsyncToPhotons(hmd);
*		printf("%f\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0.011111
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The number of seconds from Vsync to photons.
*/
float AceVR::GetHMDSecondsFromVsyncToPhotons(IVRSystem* hmd, uint32_t deviceIndex)
{
	float secondsFromVsyncToPhotons = 0.0f;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_SecondsFromVsyncToPhotons_Float;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			secondsFromVsyncToPhotons = hmd->GetFloatTrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDSecondsFromVsyncToPhotons() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDSecondsFromVsyncToPhotons() Parameters are not in valid range.";
	}

	return secondsFromVsyncToPhotons;
}


/**
* Gets the Interpupillary distance of the HMD Property
* is unique to TrackedDeviceClass_HMD.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		float retVal = AceVR::GetHMDUserIpdMeters(hmd);
*		printf("%f\n", retVal);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: 0.064000
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return The current IPD of the HMD.
*/
float AceVR::GetHMDUserIpdMeters(IVRSystem* hmd, uint32_t deviceIndex)
{
	float hmdIPDinMeters = 0.0f;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_UserIpdMeters_Float;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		if (hmd->GetTrackedDeviceClass(deviceIndex) == TrackedDeviceClass_HMD)
		{
			hmdIPDinMeters = hmd->GetFloatTrackedDeviceProperty(deviceIndex, prop);
		}
		else
		{
			throw "AceVR Error: GetHMDUserIpdMeters() deviceIndex does not refer to TrackedDeviceClass_HMD.";
		}
	}
	else
	{
		throw "AceVR Error: GetHMDUserIpdMeters() Parameters are not in valid range.";
	}

	return hmdIPDinMeters;
}

/**
* Returns true if the available firmware update is manual, false otherwise.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isUpdateManual = AceVR::GetIsFirmwareManualUpdate(hmd);
*		if (isUpdateManual == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: false
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if the available firmware update is manual, false otherwise.
*/
bool AceVR::GetIsFirmwareManualUpdate(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool isUpdateManual = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_Firmware_ManualUpdate_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		isUpdateManual = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetIsFirmwareManualUpdate() Parameters are not in valid range.";
	}

	return isUpdateManual;
}

/**
* Returns true if a firmware update is available, false otherwise.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		bool isUpdateAvailable = AceVR::GetIsFirmwareUpdateAvailable(hmd);
*		if (isUpdateAvailable == true)
*		{
*			printf("%s\n", "true");
*		}
*		else
*		{
*			printf("%s\n", "false");
*		}
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: false
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return true if a firmware update is available, false otherwise.
*/
bool AceVR::GetIsFirmwareUpdateAvailable(IVRSystem* hmd, uint32_t deviceIndex)
{
	bool isUpdateAvailable = false;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_Firmware_UpdateAvailable_Bool;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		isUpdateAvailable = hmd->GetBoolTrackedDeviceProperty(deviceIndex, prop);
	}
	else
	{
		throw "AceVR Error: GetIsFirmwareUpdateAvailable() Parameters are not in valid range.";
	}

	return isUpdateAvailable;
}

/**
* Gets the manufacturer name of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* manufacturerName = AceVR::GetManufacturerName(hmd);
*		printf("%s\n", manufacturerName);
*		_aligned_free(manufacturerName);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: HTC
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the manufacturer name.
*/
char* AceVR::GetManufacturerName(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_ManufacturerName_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetManufacturerName() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetManufacturerName() Parameters are not in valid range.";
	}

	return buffer;
}


/**
* Gets the model number of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* modelNum = AceVR::GetModelNumber(hmd);
*		printf("%s\n", modelNum);
*		_aligned_free(modelNum);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: "Vive MV"
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the model number.
*/
char* AceVR::GetModelNumber(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_ModelNumber_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetModelNumber() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetModelNumber() Parameters are not in valid range.";
	}

	return buffer;
}

/**
* Gets the render model name of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* renderModelName = AceVR::GetRenderModelName(hmd);
*		printf("%s\n", renderModelName);
*		_aligned_free(renderModelName);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: generic-hmd
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the render model name.
*/
char* AceVR::GetRenderModelName(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_RenderModelName_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetRenderModelName() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetRenderModelName() Parameters are not in valid range.";
	}

	return buffer;
}

/**
* Gets the serial number of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* serialNum = AceVR::GetSerialNumber(hmd);
*		printf("%s\n", serialNum);
*		_aligned_free(serialNum);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: LHR-F18C3E71
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the serial number.
*/
char* AceVR::GetSerialNumber(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_SerialNumber_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetSerialNumber() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetSerialNumber() Parameters are not in valid range.";
	}

	return buffer;
}

/**
* Gets the tracking system name of the specified VR device.
*
* @author Katianie
* @date 11/7/2016
*
* @code
*	try
*	{
*		EVRInitError vrInitError = VRInitError_None;
*		IVRSystem* hmd = VR_Init(&vrInitError, VRApplication_Scene);
*		if (vrInitError != VRInitError_None)
*		{
*			throw "Error: SteamVR failed to Initialize.";
*		}
*		char* trackingSystemName = AceVR::GetTrackingSystemName(hmd);
*		printf("%s\n", trackingSystemName);
*		_aligned_free(trackingSystemName);
*		VR_Shutdown();
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output: lighthouse
*
* @param [in] hmd The HMD being used.
* @param deviceIndex The index of the hardware device (HMD by default).
*
* @return A buffer containing the tracking system name.
*/
char* AceVR::GetTrackingSystemName(IVRSystem* hmd, uint32_t deviceIndex)
{
	char* buffer = NULL;
	uint32_t numBytesRead = 0;
	size_t bufferSize = vr::k_unTrackingStringSize;
	ETrackedDeviceProperty prop = ETrackedDeviceProperty::Prop_TrackingSystemName_String;

	if (hmd != NULL && deviceIndex >= 0 && deviceIndex < vr::k_unMaxTrackedDeviceCount)
	{
		//Allocate a buffer to store the corresponding property data.
		buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
		numBytesRead = hmd->GetStringTrackedDeviceProperty(deviceIndex, prop, buffer, bufferSize);

		if (numBytesRead == 0)
		{
			throw "AceVR Error: GetTrackingSystemName() Failed to get HMD information.";
		}
	}
	else
	{
		throw "AceVR Error: GetTrackingSystemName() Parameters are not in valid range.";
	}

	return buffer;
}


/**
* Returns a string that can be used to display all the
* data in the matrix.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		HmdMatrix34_t steamVrMatrix;
*		steamVrMatrix.m[0][0] = 1.0; steamVrMatrix.m[0][1] = 2.0;  steamVrMatrix.m[0][2] = 3.0;	 steamVrMatrix.m[0][3] = 4.0;
*		steamVrMatrix.m[1][0] = 5.0; steamVrMatrix.m[1][1] = 6.0;  steamVrMatrix.m[1][2] = 7.0;	 steamVrMatrix.m[1][3] = 8.0;
*		steamVrMatrix.m[2][0] = 9.0; steamVrMatrix.m[2][1] = 10.0; steamVrMatrix.m[2][2] = 11.0; steamVrMatrix.m[2][3] = 12.0;
*
*		char* steamVrMatrixToString = AceVR::SteamVRMatrix34ToString(steamVrMatrix);
*		printf("%s\n", steamVrMatrixToString);
*		_aligned_free(steamVrMatrixToString);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output:[1.000000,	2.000000,	3.000000,	4.000000]
*		 [5.000000,	6.000000,	7.000000,	8.000000]
*		 [9.000000,	10.000000,	11.000000,	12.000000]
*
* @param mat The matrix to get data from.
*
* @return A string containing the matrix data. Throws
* 		  an exception if an error occurs.
*/
char* AceVR::SteamVRMatrix34ToString(const HmdMatrix34_t& mat)
{
	int retVal;
	size_t bufferSize = 256 * sizeof(char);
	char* buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
	char* formatStr = "[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n";

	retVal = sprintf_s(buffer, bufferSize, formatStr, mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
		mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
		mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3]);

	if (retVal <= 0)
	{
		_aligned_free(buffer);
		throw "AceVR Error: SteamVRMatrix34ToString() failed to copy data to buffer.";
		return NULL;
	}
	else
	{
		return buffer;
	}
}

/**
* Returns a string that can be used to display all the
* data in the matrix.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		HmdMatrix44_t steamVrMatrix;
*		steamVrMatrix.m[0][0] = 1.0; steamVrMatrix.m[0][1] = 2.0;  steamVrMatrix.m[0][2] = 3.0;	 steamVrMatrix.m[0][3] = 4.0;
*		steamVrMatrix.m[1][0] = 5.0; steamVrMatrix.m[1][1] = 6.0;  steamVrMatrix.m[1][2] = 7.0;	 steamVrMatrix.m[1][3] = 8.0;
*		steamVrMatrix.m[2][0] = 9.0; steamVrMatrix.m[2][1] = 10.0; steamVrMatrix.m[2][2] = 11.0; steamVrMatrix.m[2][3] = 12.0;
*		steamVrMatrix.m[3][0] = 13.0; steamVrMatrix.m[3][1] = 14.0; steamVrMatrix.m[3][2] = 15.0; steamVrMatrix.m[3][3] = 16.0;
*
*		char* steamVrMatrixToString = AceVR::SteamVRMatrix34ToString(steamVrMatrix);
*		printf("%s\n", steamVrMatrixToString);
*		_aligned_free(steamVrMatrixToString);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
*
* @endcode
* Output:[1.000000,	2.000000,	3.000000,	4.000000]
*		 [5.000000,	6.000000,	7.000000,	8.000000]
*		 [9.000000,	10.000000,	11.000000,	12.000000]
*		 [13.000000, 14.000000,	15.000000,	16.000000]
*
* @param mat The matrix to get data from.
*
* @return A string containing the matrix data. Throws
* 		  an exception if an error occurs.
*/
char* AceVR::SteamVRMatrix44ToString(const HmdMatrix44_t& mat)
{
	int retVal;
	size_t bufferSize = 256 * sizeof(char);
	char* buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
	char* formatStr = "[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n";

	retVal = sprintf_s(buffer, bufferSize, formatStr, mat.m[0][0], mat.m[0][1], mat.m[0][2], mat.m[0][3],
		mat.m[1][0], mat.m[1][1], mat.m[1][2], mat.m[1][3],
		mat.m[2][0], mat.m[2][1], mat.m[2][2], mat.m[2][3],
		mat.m[3][0], mat.m[3][1], mat.m[3][2], mat.m[3][3]);

	if (retVal <= 0)
	{
		_aligned_free(buffer);
		throw "AceVR Error: SteamVRMatrix44ToString() failed to copy data to buffer.";
		return NULL;
	}
	else
	{
		return buffer;
	}
}

/**
* Returns a string that can be used to display all the
* data in the matrix.
*
* @author Katianie
* @date 11/6/2016
*
* @code
*	try
*	{
*		XMMATRIX dxMatrix;
*		dxMatrix.r[0].m128_f32[0] = 1.0; dxMatrix.r[0].m128_f32[1] = 2.0; dxMatrix.r[0].m128_f32[2] = 3.0; dxMatrix.r[0].m128_f32[3] = 4.0;
*		dxMatrix.r[1].m128_f32[0] = 5.0; dxMatrix.r[1].m128_f32[1] = 6.0; dxMatrix.r[1].m128_f32[2] = 7.0; dxMatrix.r[1].m128_f32[3] = 8.0;
*		dxMatrix.r[2].m128_f32[0] = 9.0; dxMatrix.r[2].m128_f32[1] = 10.0; dxMatrix.r[2].m128_f32[2] = 11.0; dxMatrix.r[2].m128_f32[3] = 12.0;
*		dxMatrix.r[3].m128_f32[0] = 13.0; dxMatrix.r[3].m128_f32[1] = 14.0; dxMatrix.r[3].m128_f32[2] = 15.0; dxMatrix.r[3].m128_f32[3] = 16.0;
*
*		char* dxMatrixToString = AceVR::XMMatrixToString(dxMatrix);
*		printf("%s\n", dxMatrixToString);
*		_aligned_free(dxMatrixToString);
*	}
*	catch (char* exceptionMessage)
*	{
*		printf("%s\n", exceptionMessage);
*	}
*
* @endcode
* Output:[1.000000,	2.000000,	3.000000,	4.000000]
*		 [5.000000,	6.000000,	7.000000,	8.000000]
*		 [9.000000,	10.000000,	11.000000,	12.000000]
*		 [13.000000, 14.000000,	15.000000,	16.000000]
*
* @param mat The matrix to get data from.
*
* @return A string containing the matrix data. Throws
* 		  an exception if an error occurs.
*/
char* AceVR::XMMatrixToString(const XMMATRIX& mat)
{
	int retVal;
	size_t bufferSize = 256 * sizeof(char);
	char* buffer = (char*)Ace::_Aligned_Calloc(bufferSize);
	char* formatStr = "[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n[%f,\t%f,\t%f,\t%f]\n";

	retVal = sprintf_s(buffer, bufferSize, formatStr, mat.r[0].m128_f32[0], mat.r[0].m128_f32[1], mat.r[0].m128_f32[2], mat.r[0].m128_f32[3],
		mat.r[1].m128_f32[0], mat.r[1].m128_f32[1], mat.r[1].m128_f32[2], mat.r[1].m128_f32[3],
		mat.r[2].m128_f32[0], mat.r[2].m128_f32[1], mat.r[2].m128_f32[2], mat.r[2].m128_f32[3],
		mat.r[3].m128_f32[0], mat.r[3].m128_f32[1], mat.r[3].m128_f32[2], mat.r[3].m128_f32[3]);

	if (retVal <= 0)
	{
		_aligned_free(buffer);
		throw "AceVR Error: XMMatrixToString() failed to copy data to buffer.";
		return NULL;
	}
	else
	{
		return buffer;
	}
}