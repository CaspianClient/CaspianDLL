#include <iostream>
#include <windows.h>
#include <thread>
#include <wininet.h>
#include "src/Init/Init.hpp"

DWORD WINAPI initialize(HMODULE instance) {
    if (GetConsoleWindow() == nullptr and true) {
        AllocConsole();
        SetConsoleTitleA("Caspian-Debug");
        FILE *out;
        freopen_s(&out, ("CONOUT$"), ("w"), stdout);
    }

    Init::Initialize();

    while (!Init::isDisabled()) {
        Sleep(50);
    }
    FreeLibraryAndExitThread(instance, 1);
}

BOOL APIENTRY DllMain(HMODULE instance, DWORD reason, LPVOID lpReserved)
{
    switch (reason)
    {
    case DLL_PROCESS_ATTACH: {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initialize, instance, 0, 0);
        return TRUE;
        }
    }
    return TRUE;
}