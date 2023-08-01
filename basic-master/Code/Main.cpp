#include "Common.hpp"
#include "Pointers.hpp"
#include "Fiber.hpp"
#include "Hooking.hpp"


BOOL DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved)
{
	if (ul_reason_for_call == DLL_PROCESS_ATTACH)
	{
		g_Module = hModule;
		g_MainThread = CreateThread(nullptr, 0, [](LPVOID) -> DWORD
			{
				g_Logger = new Logger();
				
				g_Logger->Custom("Init", BASE_NAME " Injected");
				
				g_Pointers = new Pointers();
				g_Invoker = new Invoker();
				
				g_Pointers->PostInit();
				
				g_Fiber = new Fiber();
				g_Hooking = new Hooking();
				
				g_Hooking->Hook();
				
				while (g_Running) {
					if (GetAsyncKeyState(VK_F12)) g_Running = false;
				}
				
				g_Hooking->Unhook();
				
				delete g_Hooking;
				delete g_Fiber;
				delete g_Invoker;
				delete g_Pointers;
				
				g_Logger->Info(BASE_NAME " Unloaded");
				std::this_thread::sleep_for(500ms);
				
				delete g_Logger;

				CloseHandle(g_MainThread);
				FreeLibraryAndExitThread(g_Module, 0);
			}, nullptr, 0, nullptr);
	}

	return true;
}
