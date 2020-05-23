// dllmain.cpp : Defines the entry point for the DLL application.
#include "pch.h"
#include "injector/injector.hpp"
#include "ini/IniReader.h"

void Init()
{
	CIniReader iniReader("AutosculptFix.ini");
	bool MakeWheelsRotated = iniReader.ReadInteger((char*)"GENERAL", (char*)"MakeWheelsRotated", 1);

	injector::MakeNOP(0x007E3544, 10, true);

	char fixAutosculptRimRender[3] = { 0xB1, 0x01, 0x90 };
	injector::WriteMemoryRaw(0x007DE6E3, fixAutosculptRimRender, 3, true);

	if (MakeWheelsRotated)
	{
		char makeWheelsRotated[3] = { 0xB0, 0x00, 0x90 };
		injector::WriteMemoryRaw(0x007E585D, makeWheelsRotated, 3, true);
	}
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		uintptr_t base = (uintptr_t)GetModuleHandleA(NULL);
		IMAGE_DOS_HEADER* dos = (IMAGE_DOS_HEADER*)(base);
		IMAGE_NT_HEADERS* nt = (IMAGE_NT_HEADERS*)(base + dos->e_lfanew);

		if ((base + nt->OptionalHeader.AddressOfEntryPoint + (0x400000 - base)) == 0x87E926) // Check if .exe file is compatible - Thanks to thelink2012 and MWisBest
		{
			Init();
		}
		else
		{
			MessageBoxA(NULL, "This .exe is not supported.\nPlease use v1.4 English nfsc.exe (6,88 MB (7.217.152 bytes)).", "NFSC - AutosculptFix", MB_ICONERROR);
			return FALSE;
		}
	}
	break;
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		break;
	}
	return TRUE;
}

