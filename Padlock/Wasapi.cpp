#include <mmdeviceapi.h>
#include <psapi.h>

#include "Wasapi.h"

ISimpleAudioVolume* GetSpotifySimpleAudioVolume()
{
	// not sure what the second parameter should be here, but this works for now
	//
	// also this might not be needed as winrt::init_apartment() is called in wWinMain()
	//HRESULT hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);

	// any of these pointers could be null due to an error, so this is a bit risky
	IMMDeviceEnumerator* deviceEnumerator;
	HRESULT hr = CoCreateInstance(
		CLSID_MMDeviceEnumerator, NULL,
		CLSCTX_ALL, IID_IMMDeviceEnumerator,
		(void**)&deviceEnumerator);

	IMMDevice* mmDevice;
	deviceEnumerator->GetDefaultAudioEndpoint(eRender, eMultimedia, &mmDevice);

	IAudioSessionManager2* audioSessionManager2;
	mmDevice->Activate(IID_IAudioSessionManager2, CLSCTX_ALL, NULL, (void**)&audioSessionManager2);

	IAudioSessionEnumerator* audioSessionEnumerator;
	audioSessionManager2->GetSessionEnumerator(&audioSessionEnumerator);

	int sessionCount;
	audioSessionEnumerator->GetCount(&sessionCount);
	for (int sessionNum = 0; sessionNum < sessionCount; ++sessionNum) {
		IAudioSessionControl* audioSessionControl;
		audioSessionEnumerator->GetSession(sessionNum, &audioSessionControl);

		IAudioSessionControl2* audioSessionControl2;
		audioSessionControl->QueryInterface(IID_IAudioSessionControl2, (void**)&audioSessionControl2);

		DWORD pid;
		audioSessionControl2->GetProcessId(&pid);

		auto name = std::wstring{ GetProcessNameFromPid(pid) };
		
		if (name == L"Spotify.exe") {

			ISimpleAudioVolume* simpleAudioVolume;
			audioSessionControl2->QueryInterface(IID_ISimpleAudioVolume, (void**)&simpleAudioVolume);

			// TODO: find a way to remove code duplication here and at the end of the func and loop(goto?)
			audioSessionControl->Release();
			audioSessionControl2->Release();
			deviceEnumerator->Release();
			mmDevice->Release();
			audioSessionManager2->Release();
			audioSessionEnumerator->Release();

			return simpleAudioVolume;
		}

		audioSessionControl->Release();
		audioSessionControl2->Release();
	}

	deviceEnumerator->Release();
	mmDevice->Release();
	audioSessionManager2->Release();
	audioSessionEnumerator->Release();

	// see the comment in GetSpotifyGsmtcSession()
	return nullptr;
}

LPCWSTR GetProcessNameFromPid(DWORD pid)
{
	HANDLE handle = OpenProcess(
		PROCESS_QUERY_INFORMATION | PROCESS_VM_READ,
		FALSE,
		pid
	);

	WCHAR baseName[MAX_PATH] = { 0 };
	GetModuleBaseName(handle, NULL, baseName, MAX_PATH);

	CloseHandle(handle);

	return baseName;
}
