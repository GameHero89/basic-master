#include "Common.hpp"
#include "Fiber.hpp"
#include "Keyboard.hpp"
#include "Natives.hpp"
#include "Interface.hpp"

	void WINAPI FiberFunction(LPVOID lpParameter)
	{
		try {
			g_Logger->Custom("Init", BASE_NAME" Loaded");

			while (true) {
				Interface();
				g_Fiber->Wait();
			}
		}
		catch (...) {
			g_Logger->Info("Failed scriptFiber");
		}
	}

	Fiber::Fiber() {
		ScriptFiber = CreateFiber(NULL, FiberFunction, nullptr);
		g_Logger->Custom("Init", "Initialized -> scriptFiber");
	}

	Fiber::~Fiber() {
		DeleteFiber(ScriptFiber);
	}

	void Fiber::OnTick()
	{
		if (MainFiber == nullptr) {
			MainFiber = IsThreadAFiber() ? GetCurrentFiber() : ConvertThreadToFiber(nullptr);
		}

		if (WaitTime <= timeGetTime()) {
			if (ScriptFiber != nullptr) {
				SwitchToFiber(ScriptFiber);
			}
		}
	}

	void Fiber::Wait(uint32_t ms) {
		WaitTime = timeGetTime() + ms;
		SwitchToFiber(MainFiber);
	}