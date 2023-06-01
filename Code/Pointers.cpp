#include "Common.hpp"
#include "Pointers.hpp"
#include "Memory.hpp"

Pointers::Pointers() 
{
	m_VectorFix = Signature("vector_fix", "83 7A 18 00 75 B6").Scan().Sub(77).As<decltype(m_VectorFix)>();
	//m_ModelBypass = Signature("model_bypass", "74 6B 48 85 D2").Scan().Add(546).As<decltype(m_ModelBypass)>();
	m_GameState = Signature("game_state", "48 85 C9 74 4B 83 3D").Scan().Add(7).Rip().As<decltype(m_GameState)>();
	m_NativeRegistrations = Signature("native_registration", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").Scan().Add(3).Rip().As<decltype(m_NativeRegistrations)>();
	m_GetNativeHandler = Signature("get_native_handler", "48 8D 0D ? ? ? ? 48 8B 14 FA E8 ? ? ? ? 48 85 C0 75 0A").Scan().Add(12).Rip().As<decltype(m_GetNativeHandler)>();
	m_GlobalBase = Signature("global_base", "4C 8D 4D 08 48 8D 15 ? ? ? ? 4C 8B C0").Scan().Add(7).Rip().As<decltype(m_GlobalBase)>();
	m_RunScriptThread = Signature("run_script_thread", "45 33 F6 8B E9 85 C9 B8").Scan().Sub(0x1F).As<decltype(m_RunScriptThread)>();
}

void Pointers::PostInit()
{

}
