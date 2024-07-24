#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Control.h>

#include <audiopolicy.h>
#include <mmdeviceapi.h>

using namespace winrt::Windows::Media::Control;
using namespace winrt::Windows::Foundation;

LPCWSTR GetProcessNameFromPid(const DWORD pid);

class Spotify
{
public:
	static void NextTrack();
	static void PreviousTrack();
	static void TogglePlayPause();

	static void ReduceVolumeBy(const float amount);
	static void IncreaseVolumeBy(const float amount);

	static GlobalSystemMediaTransportControlsSession GetSpotifyGsmtcSession();
};

static const LPCWSTR        SPOTIFY_EXE_NAME = L"Spotify.exe";

const int     PADLOCK_HOTKEY_ID = 7;
const LPCWSTR PADLOCK_WND_CLASS_NAME = L"Padlock";
const float   PADLOCK_VOLUME_DELTA = 0.05f;

const IID   IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID   IID_IAudioSessionManager2 = __uuidof(IAudioSessionManager2);
const IID   IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);
const IID   IID_ISimpleAudioVolume = __uuidof(ISimpleAudioVolume);
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
