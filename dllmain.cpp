#include <Windows.h>
#include "MinHook.h"
#include <cstring> // Include for strcmp

// Function pointer for original setString function
typedef void(__thiscall* SetStringFunc)(void* thisptr, const char* text);
SetStringFunc cocos2d_CCLabelBMFont_setString = nullptr;

// Hooked function
void __fastcall cocos2d_CCLabelBMFont_setString_H(void* thisptr, void*, const char* text)
{
    if (strcmp(text, "Replay by aBot") == 0) {
        text = "";
    }

    // Call original function
    cocos2d_CCLabelBMFont_setString(thisptr, text);
}

// Main thread function
DWORD WINAPI ThreadMain(LPVOID lpParam)
{
    MH_Initialize();

    // Get function address
    auto setStringFunction = (void*)GetProcAddress(GetModuleHandleA("libcocos2d.dll"), "?setString@CCLabelBMFont@cocos2d@@UAEXPBD@Z");
    if (!setStringFunction) return 1;

    // Hook function
    MH_CreateHook((LPVOID)setStringFunction, (LPVOID)&cocos2d_CCLabelBMFont_setString_H, reinterpret_cast<LPVOID*>(&cocos2d_CCLabelBMFont_setString));
    MH_EnableHook((LPVOID)setStringFunction);

    return 0;
}

// DLL entry point
BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
    if (ul_reason_for_call == DLL_PROCESS_ATTACH)
    {
        HANDLE hThread = CreateThread(0, 0, &ThreadMain, 0, 0, 0);
        if (hThread) CloseHandle(hThread);
    }
    return TRUE;
}
