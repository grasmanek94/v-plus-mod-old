#include "DllMain.h"

bool CGameOverlay::bInitialized = false;

ID3D11Device * CGameOverlay::pDevice = NULL;
ID3D11DeviceContext * CGameOverlay::pContext = NULL;

DXGISwapChainPresent CGameOverlay::pRealPresent = NULL;
uintptr_t CGameOverlay::hkSwapChainVFTable[64];

IFW1Factory * CGameOverlay::pFW1Factory = NULL;
IFW1FontWrapper * CGameOverlay::pFontWrapper = NULL;

HRESULT __stdcall CGameOverlay::HookedPresent(IDXGISwapChain* pSwapChain, UINT SyncInterval, UINT Flags)
{
    if(!bInitialized)
    {
        pSwapChain->GetDevice(__uuidof(pDevice), (void**)&pDevice);
        pDevice->GetImmediateContext(&pContext);

        FW1CreateFactory(FW1_VERSION, &pFW1Factory);
        pFW1Factory->CreateFontWrapper(pDevice, L"Tahoma", &pFontWrapper);

        pFW1Factory->Release();

        bInitialized = true;
    }

	wchar_t wszGameStateString[128];
	swprintf_s(wszGameStateString, 128, L"Game state: %d", CGameStateWatcher::GetGameState());

	pFontWrapper->DrawString(pContext, wszGameStateString, 18.0f, 22.0f, 20.0f, 0xff000000, FW1_RESTORESTATE);
	pFontWrapper->DrawString(pContext, wszGameStateString, 18.0f, 20.0f, 18.0f, 0xffffffff, FW1_RESTORESTATE);

	if(CGameStateWatcher::GetGameState() == GameStatePlaying && CGameAddresses::ppWorld != NULL)
	{
		uint64_t pWorld = *(uint64_t *)(CGameAddresses::ppWorld);

		if(pWorld != NULL)
		{
			uint64_t pLocalPlayerPed = *(uint64_t *)(pWorld + 8);

			if(pLocalPlayerPed != NULL)
			{
				wchar_t wszPositionString[128];
				swprintf_s(wszPositionString, 128, L"Position: %.2f, %.2f, %.2f",
					*(float *)(pLocalPlayerPed + 0x90), *(float *)(pLocalPlayerPed + 0x94), *(float *)(pLocalPlayerPed + 0x98));

				pFontWrapper->DrawString(pContext, wszPositionString, 18.0f, 22.0f, 42.0f, 0xff000000, FW1_RESTORESTATE);
				pFontWrapper->DrawString(pContext, wszPositionString, 18.0f, 20.0f, 40.0f, 0xffffffff, FW1_RESTORESTATE);
			}
		}
	}

    return pRealPresent(pSwapChain, SyncInterval, Flags);
}

bool CGameOverlay::Setup()
{
	auto location = CGameUtility::FindPattern("\x80\x7E\x10\x00\x48\x8B\x0D\x00\x00\x00\x00\x48\x8D\x94\x24\xA0\x00\x00\x00\x0F\x94\x05", "xxxxxxx????xxxxxxxxxxx");

	while(!location)
	{
		location = CGameUtility::FindPattern("\x80\x7E\x10\x00\x48\x8B\x0D\x00\x00\x00\x00\x48\x8D\x94\x24\xA0\x00\x00\x00\x0F\x94\x05", "xxxxxxx????xxxxxxxxxxx");

		Sleep(50);
	}

	if(location != 0)
	{
		uintptr_t ppDXGISwapChain = (uintptr_t)((location + 7) + *(unsigned int *)(location + 7) + 4);

		if(ppDXGISwapChain == NULL)
		{
			return false;
		}

		IDXGISwapChain *pSwapChain = *(IDXGISwapChain **)(ppDXGISwapChain);

		if(pSwapChain != NULL)
		{
			uintptr_t realSwapChainVFTable = *(uintptr_t *)(pSwapChain);
			DWORD dwProt1 = NULL, dwProt2 = NULL;

			VirtualProtect((LPVOID)(realSwapChainVFTable), 512, PAGE_EXECUTE_READWRITE, &dwProt1);
			memcpy(&hkSwapChainVFTable, (const void *)(realSwapChainVFTable), 512);
			VirtualProtect((LPVOID)(realSwapChainVFTable), 512, dwProt1, &dwProt2);

			pRealPresent = (DXGISwapChainPresent)hkSwapChainVFTable[8];
			hkSwapChainVFTable[8] = (uintptr_t)HookedPresent;

			VirtualProtect((LPVOID)(pSwapChain), 4, PAGE_EXECUTE_READWRITE, &dwProt1);
			*(uintptr_t *)(pSwapChain) = (uintptr_t)&hkSwapChainVFTable;
			VirtualProtect((LPVOID)(pSwapChain), 4, dwProt1, &dwProt2);
		}

		return true;
	}

	return false;
}

void CGameOverlay::Shutdown()
{
	if(pFontWrapper)
	{
		pFontWrapper->Release();
	}
}
