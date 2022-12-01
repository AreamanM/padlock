#pragma once

#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.Media.Control.h>

#include <audiopolicy.h>
#include <mmdeviceapi.h>

using namespace winrt::Windows::Media::Control;
using namespace winrt::Windows::Foundation;

LPCWSTR GetProcessNameFromPid(const DWORD pid);

// a class representing the Spotify desktop client
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

// yes, these are both the same, but they could change later
static const LPCWSTR        SPOTIFY_EXE_NAME = L"Spotify.exe";
// static const winrt::hstring SPOTIFY_SOURCE_APP_USER_MODEL_ID = L"Spotify.exe"; <- desktop app only(?)

// TODO: figure out a way to make this unique
//
// 7 because there are 7 letters in 'padlock'
const int     PADLOCK_HOTKEY_ID = 7;
const LPCWSTR PADLOCK_WND_CLASS_NAME = L"Padlock";
// the amount the volume will change, in this case, 5 points out of a 100
const float   PADLOCK_VOLUME_DELTA = 0.05f;

// some of these are already predefined but it's a good idea to define all of these here anyway because
// sometimes you end up with a linker error if you use the predfined ones in the windows headers
const IID   IID_IMMDeviceEnumerator = __uuidof(IMMDeviceEnumerator);
const IID   IID_IAudioSessionManager2 = __uuidof(IAudioSessionManager2);
const IID   IID_IAudioSessionControl2 = __uuidof(IAudioSessionControl2);
const IID   IID_ISimpleAudioVolume = __uuidof(ISimpleAudioVolume);
const CLSID CLSID_MMDeviceEnumerator = __uuidof(MMDeviceEnumerator);
