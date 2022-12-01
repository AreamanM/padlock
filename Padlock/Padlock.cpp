#include <sstream>
#include <chrono>

#include "padlock.h"
#include "Logging.h"
#include "Wasapi.h"

#define LOG_PATH std::filesystem::path{ getenv("TEMP") } / std::filesystem::path{ "Padlock.log" }
#define LOG_EVENT(msg) do {\
		auto t = std::time(nullptr);\
		std::ostringstream o;\
		o << "[" << std::put_time(std::localtime(&t), "%d/%m/%Y %H:%M:%S") << "] " << std::string{ msg } << std::endl;\
		auto tmp = o.str();\
		LogAppend(tmp, LOG_PATH);\
	} while (false)

using namespace winrt::Windows::Media::Control;

int WINAPI wWinMain(
	_In_     HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_     LPWSTR    lpCmdLine,
	_In_     int       nShowCmd
)
{
	UNREFERENCED_PARAMETER(hInstance);
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	UNREFERENCED_PARAMETER(nShowCmd);

	winrt::init_apartment();

	LogInit(LOG_PATH);

	for (auto key : { VK_LEFT, VK_RIGHT, VK_DOWN, VK_UP, VK_SPACE }) {
		RegisterHotKey(NULL, PADLOCK_HOTKEY_ID, MOD_CONTROL | MOD_ALT, key);
		LOG_EVENT("Registered hotkey");
	}

	MSG msg{};
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		if (msg.message == WM_HOTKEY && msg.wParam == PADLOCK_HOTKEY_ID) {
			switch (HIWORD(msg.lParam)) {
			case VK_DOWN:
				Spotify::ReduceVolumeBy(PADLOCK_VOLUME_DELTA);
				LOG_EVENT("ReduceVolumeBy");
				break;
			case VK_UP:
				Spotify::IncreaseVolumeBy(PADLOCK_VOLUME_DELTA);
				LOG_EVENT("IncreaseVolumeBy");
				break;
			case VK_LEFT:
				Spotify::PreviousTrack();
				LOG_EVENT("PreviousTrack");
				break;
			case VK_RIGHT:
				Spotify::NextTrack();
				LOG_EVENT("NextTrack");
				break;
			case VK_SPACE:
				Spotify::TogglePlayPause();
				LOG_EVENT("TogglePlayPause");
				break;
			}
		}
	}
}

GlobalSystemMediaTransportControlsSession Spotify::GetSpotifyGsmtcSession()
{
	// ideally there should be some kind of caching system in place for this function
	auto sessionManager = GlobalSystemMediaTransportControlsSessionManager::RequestAsync().get();
	for (GlobalSystemMediaTransportControlsSession s : sessionManager.GetSessions()) {
		// std::ostringstream os;
		// os << s.SourceAppUserModelId() << "\n";
		if (s.SourceAppUserModelId() == L"SpotifyAB.SpotifyMusic_zpdnekdrzrea0!Spotify"
				|| s.SourceAppUserModelId() == L"Spotify")
			return s;
	}
	// using nullptr wherever possible(instead of NULL) is more sensible
	return nullptr;
}


void Spotify::PreviousTrack()
{
	// as mentioned in Spotify::GetSpotifyGsmtcSession(), some caching here(and for the 2 other related funcs) would be ideal
	if (auto s = Spotify::GetSpotifyGsmtcSession(); s != nullptr)
		s.TrySkipPreviousAsync().get();
}

void Spotify::NextTrack()
{
	if (auto s = Spotify::GetSpotifyGsmtcSession(); s != nullptr)
		s.TrySkipNextAsync().get();
}

void Spotify::TogglePlayPause()
{
	if (auto s = Spotify::GetSpotifyGsmtcSession(); s != nullptr)
		s.TryTogglePlayPauseAsync().get();
}

void Spotify::ReduceVolumeBy(const float amount)
{
	if (auto audioVolume = GetSpotifySimpleAudioVolume(); audioVolume != nullptr) {
		float currentVolume;
		audioVolume->GetMasterVolume(&currentVolume);

		if (currentVolume - amount < 0)
			currentVolume = amount;

		audioVolume->SetMasterVolume(currentVolume - amount, NULL);
		audioVolume->Release();
	}
}

void Spotify::IncreaseVolumeBy(const float amount)
{
	if (auto audioVolume = GetSpotifySimpleAudioVolume(); audioVolume != nullptr) {
		float currentVolume;
		audioVolume->GetMasterVolume(&currentVolume);

		if (currentVolume + amount > 1)
			currentVolume = 1 - amount;

		audioVolume->SetMasterVolume(currentVolume + amount, NULL);
		audioVolume->Release();
	}
}
