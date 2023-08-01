#pragma once

class Hooking
{
public:
	Hooking();
	~Hooking();

	void Hook();
	void Unhook();
};

inline Hooking* g_Hooking;
