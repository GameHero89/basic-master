#include "Common.hpp"
#include "Invoker.hpp"
#include "Pointers.hpp"
#include "Crossmap.hpp"

Invoker::Invoker() {
	g_Logger->Custom("Crossmap", "Checking %i Hashes for Natives", ARRAYSIZE(g_Crossmap));
	int trys = 0;
	while (!m_HandlerCache.size() && trys < 5) {
		for (auto mapping : g_Crossmap) {
			void* handler = g_Pointers->m_GetNativeHandler(g_Pointers->m_NativeRegistrations, mapping.second);
			if (handler == nullptr) {
				g_Logger->Custom("Crossmap", "0x%p -> 0x%p is invalid", mapping.first, mapping.second);
				continue;
			}
			m_HandlerCache.emplace(mapping.first, handler);
		}
		trys++;
		if (trys > 2) g_Logger->Info("Waiting for Natives...");
		if (!m_HandlerCache.size()) std::this_thread::sleep_for(2s);
	}
	g_Logger->Custom("Crossmap", "Valid Natives Found %d / %d                  ", m_HandlerCache.size(), ARRAYSIZE(g_Crossmap));
	if (ARRAYSIZE(g_Crossmap) - m_HandlerCache.size() > 50) {
		g_Logger->Error("Failed at Crossmap!");
	}
}

void* Invoker::GetHandler(rage::scrNativeHash hash) {
	if (auto it = m_HandlerCache.find(hash); it != m_HandlerCache.end()) {
		return it->second;
	}
	return nullptr;
}

void Invoker::EndCall(rage::scrNativeHash hash) {
	void* handler = GetHandler(hash);
	if (handler != nullptr)
	{
		__try 
		{
			static_cast<void(*)(rage::scrNativeCallContext*)>(handler)(&m_CallContext);
			g_Pointers->m_VectorFix(&m_CallContext);
		}
		__except (EXCEPTION_EXECUTE_HANDLER)
		{
			g_Logger->Info("Exception caught while trying to call 0x%x native.", hash);
		}
	}
	else {
		g_Logger->Info("Failed to find 0x%x native's handler.", hash);
	}
}