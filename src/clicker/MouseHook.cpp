#include "pch.h"
#include "MouseHook.h"
#include "../toad.h"

HHOOK hook = NULL;
MSG msg;

// #TODO: doesn't need its own thread, move it to the main thread 
void hotkey_listener_thread()
{
    while (toad::is_running) {
        if (toad::clicker::slot_whitelist && toad::clicker::enabled && toad::window_is_focused(toad::clicking_window)) {
            for (int i = 0; i < toad::hotbar_virtual_keycodes.size(); i++)
            {
                if (GetAsyncKeyState(toad::hotbar_virtual_keycodes[i]) & 1)
                {
                    toad::clicker::curr_slot = i;
                }
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1));
        }
        else std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}

LRESULT _stdcall mousecallback(int nCode, WPARAM wParam, LPARAM lParam)
{
	auto pMouseStruct = reinterpret_cast<MSLLHOOKSTRUCT*>(lParam);

    if (nCode == 0)
    {
        switch (wParam)
        {
        case WM_MOUSEWHEEL:
            if ((int)pMouseStruct->mouseData < 0) toad::clicker::curr_slot == 0 ? toad::clicker::curr_slot = 8 : toad::clicker::curr_slot--;
            else if ((int)pMouseStruct->mouseData > 0) toad::clicker::curr_slot == 8 ? toad::clicker::curr_slot = 0 : toad::clicker::curr_slot++;
            break;
        case WM_LBUTTONDOWN:
        case WM_LBUTTONUP:
	        p_clickRecorder->save_delay();
            break;
        default:
            break;
        }
    }
    return CallNextHookEx(hook, nCode, wParam, lParam);
}

void MouseHook::Thread()
{
    //init hook
    //LOG_DEBUG("hooking mouse");
    hook = SetWindowsHookEx(WH_MOUSE_LL, mousecallback, NULL, 0);
    //hook ? LOG_DEBUG("hook successful") : LOG_ERROR("failed to set hook");

    std::thread(hotkey_listener_thread).detach();

    while (toad::is_running)
    {
        if (GetMessage(&msg, NULL, 0, 0))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }       
    }
}

void MouseHook::Unhook()
{
#ifndef _DEBUG
    LOG_DEBUG("Unhooking mouse");
    //Unhook
    UnhookWindowsHookEx(hook);
#endif
}
