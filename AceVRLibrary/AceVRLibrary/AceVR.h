#pragma once
#include <fstream>
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <process.h>
#include <vector>
#include <cmath>
#include <algorithm>
#include <iostream>
#include <iomanip>

///Windows
#include <windows.h>
#include <WinBase.h>
#define WIN32_LEAN_AND_MEAN 

///DirectX
#include <d3d11.h>
#include <DirectXColors.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

///OpenVR
#include <openvr.h>

#pragma comment (lib, "dxgi.lib")
#pragma comment (lib, "d3d11.lib")
#pragma comment (lib, "d3dcompiler.lib")
#pragma comment (lib, "openvr_api.lib")
#pragma comment(lib, "advapi32.lib")

using namespace std;
using namespace vr;
using namespace DirectX;

///Common
#include "Ace.h"

class AceVR
{
public:

	static XMMATRIX ConvertSteamVRMatrixToXMMATRIX(const HmdMatrix34_t& matPose);
	static bool		GetContainsProximitySensor(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetControllerIsButtonPressed(IVRSystem* hmd, EVRButtonId button);
	static bool		GetControllerIsButtonTouched(IVRSystem* hmd, EVRButtonId button);
	static XMMATRIX GetCurrentViewProjectionMatrix(IVRSystem* hmd, const XMMATRIX& projectionMat, const XMMATRIX& eyePose, const XMMATRIX& hmdPose);
	static float	GetDeviceBatteryPercentage(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetDeviceClass(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetDeviceIsCharging(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetDeviceIsWireless(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetDeviceProvidesBatteryStatus(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetFirmwareProgrammingTarget(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetFPGAVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetHasCamera(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDAudioFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDCameraCompatibilityMode(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDCurrentUniverseId(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDDisplayBootloaderVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDDisplayFirmwareVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static float	GetHMDDisplayFrequency(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static uint64_t	GetHMDDisplayHardwareVersion(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static XMMATRIX GetHMDEyePoseMatrix(IVRSystem* hmd, Hmd_Eye nEye);
	static XMMATRIX GetHMDEyeProjectionMatrix(IVRSystem* hmd, Hmd_Eye nEye, float nearClipPlane, float farClipPlane);
	static uint64_t	GetHMDPreviousUniverseId(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static float	GetHMDSecondsFromVsyncToPhotons(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static float	GetHMDUserIpdMeters(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetIsFirmwareManualUpdate(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static bool		GetIsFirmwareUpdateAvailable(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetManufacturerName(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetModelNumber(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetRenderModelName(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetSerialNumber(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	GetTrackingSystemName(IVRSystem* hmd, uint32_t deviceIndex = vr::k_unTrackedDeviceIndex_Hmd);
	static char*	SteamVRMatrix34ToString(const HmdMatrix34_t& mat);
	static char*	SteamVRMatrix44ToString(const HmdMatrix44_t& mat);
	static char*	XMMatrixToString(const XMMATRIX& mat);
};