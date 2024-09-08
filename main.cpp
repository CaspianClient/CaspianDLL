#include <iostream>
#include <windows.h>
#include <thread>
#include <wininet.h>
#include "src/Caspian/Init/Init.hpp"

#include <winrt/windows.foundation.h>
#include <winrt/windows.ui.h>
#include <winrt/windows.ui.notifications.h>
#include <winrt/windows.ui.notifications.management.h>
#include <winrt/windows.ui.popups.h>
#include <winrt/windows.data.xml.dom.h>

using namespace winrt::Windows::UI::Notifications;
using namespace winrt::Windows::UI::Popups;
using namespace winrt::Windows::Data::Xml::Dom;

DWORD WINAPI initialize(HMODULE instance) {
    if (GetConsoleWindow() == nullptr and false) {
        AllocConsole();
        SetConsoleTitleA("Caspian-Debug");
        FILE *out;
        freopen_s(&out, ("CONOUT$"), ("w"), stdout);
    }

    Init::Initialize();

    winrt::init_apartment();

    ToastNotification toastNotif = ToastNotification(ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastImageAndText02));
	IXmlNodeList txtElmnt = toastNotif.Content().GetElementsByTagName(L"text");
	txtElmnt.Item(0).InnerText(winrt::to_hstring("Caspian Client"));
	txtElmnt.Item(1).InnerText(winrt::to_hstring("Caspian has been injected!"));
	ToastNotificationManager::CreateToastNotifier().Show(toastNotif);

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