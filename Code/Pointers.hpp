#pragma once
#include "Types.hpp"
#include "Invoker.hpp"

class Pointers
{
public:
	Pointers();
	void PostInit();

	using IsDlcPresent = bool(std::uint32_t hash);
	IsDlcPresent* m_IsDlcPresent;

	using FixVectors = void(rage::scrNativeCallContext* ctx);
	FixVectors* m_VectorFix;

	using GetNativeHandler = void*(rage::scrNativeRegistrationTable*, rage::scrNativeHash);
	GetNativeHandler* m_GetNativeHandler;

	using run_script_threads = bool(*)(std::uint32_t ops_to_execute);
	run_script_threads m_RunScriptThread;

	PVOID m_ModelBypass;
	std::uint64_t m_CPedFactory;
	std::uint32_t* m_GameState;
	rage::scrNativeRegistrationTable* m_NativeRegistrations;
	std::uint64_t** m_GlobalBase;
};

inline Pointers* g_Pointers;
