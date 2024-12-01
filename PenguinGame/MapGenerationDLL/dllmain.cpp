#include "pch.h"
#include "framework.h"
#include "MapGeneration.h"  // Include fișierul header al DLL-ului

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved)
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
        // Inițializăm ceva, dacă e nevoie (de exemplu, setăm semnături aleatorii, etc.)
        break;
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Curățăm orice resurse, dacă e cazul
        break;
    }
    return TRUE;
}


