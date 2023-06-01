#include "Common.hpp"
#include "Natives.hpp"
#include "Fiber.hpp"
#include "Hooking.hpp"
#include "Pointers.hpp"
#include <MinHook/MinHook.h>

#pragma comment(lib, "MinHook-Release.lib")

std::vector<void*> m_Hooks;

void MH_CreateDetour(const char* Name, LPVOID Target, LPVOID Detour, LPVOID* Original) {
	void* m_OriginalFunction;
	MH_STATUS status = MH_CreateHook(Target, Detour, Original);
	if (status != MH_OK && status != MH_ERROR_ALREADY_CREATED) {
		g_Logger->Custom("Detour", "Failed -> %s", Name);
	} else {
		g_Logger->Custom("Detour", "Hooked -> %s", Name);
	}
	m_Hooks.push_back(Target);
}

void* OriginalRunScriptThread;
bool RunScriptThreadHook(uint32_t hash) {
	if (g_Running)
	{
		g_Fiber->OnTick();
	}
	return static_cast<decltype(&RunScriptThreadHook)>(OriginalRunScriptThread)(hash);
}


Hooking::Hooking() {
	MH_Initialize();
	MH_CreateDetour("RUN_SCRIPT_THREAD", g_Pointers->m_RunScriptThread, &RunScriptThreadHook, &OriginalRunScriptThread);
}

Hooking::~Hooking() {
	for (void* Hook : m_Hooks) {
		MH_RemoveHook(Hook);
	}
	MH_Uninitialize();
}

void Hooking::Hook() {
	MH_EnableHook(MH_ALL_HOOKS);
}

void Hooking::Unhook() {
	MH_DisableHook(MH_ALL_HOOKS);
}



