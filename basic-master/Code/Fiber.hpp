#pragma once


class Fiber
{
public:
	Fiber();
	~Fiber();
	void OnTick();
	void Wait(uint32_t ms = 0);
private:
	uint64_t WaitTime;
	HANDLE MainFiber;
	HANDLE ScriptFiber;
};

inline Fiber* g_Fiber;



