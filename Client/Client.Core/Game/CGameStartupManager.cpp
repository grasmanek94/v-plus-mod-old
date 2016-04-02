#include "DllMain.h"

uint32_t * CGameStartupManager::pLandingPage = NULL;
uint32_t * CGameStartupManager::pStartupFlow = NULL;

uint32_t CGameStartupManager::originalSettings[2] = { 0, 0 };

DWORD WINAPI CGameStartupManager::WorkerThread()
{
	if(!pLandingPage || !pStartupFlow || CGameStateWatcher::GetGameState() == GameStateUnknown)
	{
		return 0;
	}

	while(CGameStateWatcher::GetGameState() != GameStateLegalScreen)
	{
		Sleep(50);
	}

	originalSettings[0] = *pLandingPage;
	originalSettings[1] = *pStartupFlow;

	while(CGameStateWatcher::GetGameState() != GameStatePlaying)
	{
		*pLandingPage = 0;
		*pStartupFlow = 0;

		Sleep(50);
	}

	*pLandingPage = originalSettings[0];
	*pStartupFlow = originalSettings[1];
	return 1;
}

bool CGameStartupManager::Setup()
{
	auto landingPageLocation = CGameUtility::FindPattern("\xBA\x2A\x03\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x44\x8B\x05\x00\x00\x00\x00\xBA\x2B\x03\x00\x00\x48\x8B\xCB", "xxxxxxxxx????xxx????xxxxxxxx");
	auto startupFlowLocation = CGameUtility::FindPattern("\xBA\xE3\x00\x00\x00\x48\x8B\xCB\xE8\x00\x00\x00\x00\x44\x8B\x05\x00\x00\x00\x00\xBA\x2A\x03\x00\x00\x48\x8B\xCB", "xxxxxxxxx????xxx????xxxxxxxx");

	if(landingPageLocation != NULL && startupFlowLocation != NULL)
	{
		pLandingPage = (uint32_t *)((landingPageLocation + 16) + *(unsigned int *)(landingPageLocation + 16) + 4);
		pStartupFlow = (uint32_t *)((startupFlowLocation + 16) + *(unsigned int *)(startupFlowLocation + 16) + 4);

		if(pLandingPage != NULL && pStartupFlow != NULL)
		{
			CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)WorkerThread, NULL, NULL, NULL);
		}
	}

	return true;
}
