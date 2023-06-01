#pragma once

#define _CRT_SECURE_NO_WARNINGS
#define NOMINMAX
#define WIN32_LEAN_AND_MEAN
#define _WIN32_WINNT _WIN32_WINNT_WIN7
#pragma comment(lib, "Winmm.lib")
#pragma comment(lib, "ws2_32.lib")

#include <winsdkver.h>
#include <sdkddkver.h>
#include <windows.h>
#include <winternl.h>
#include <timeapi.h>
#include <d3d11.h>

#include <cassert>
#include <cinttypes>
#include <cstdarg>
#include <cstddef>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <ctime>

#include <algorithm>
#include <atomic>
#include <chrono>
#include <deque>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iomanip>
#include <iostream>
#include <limits>
#include <memory>
#include <mutex>
#include <optional>
#include <sstream>
#include <stack>
#include <string>
#include <thread>
#include <fibersapi.h>
#include <type_traits>
#include <unordered_map>
#include <vector>

#define BASE_NAME "It's Basic"

using namespace std::chrono_literals;
namespace fs = std::filesystem;

inline HMODULE g_Module;
inline HANDLE g_MainThread;
inline std::atomic_bool g_Running = true;
inline std::uint32_t g_SelectedPlayer;

#include "Types.hpp"
#include "Logger.hpp"
#include "ScriptGlobal.hpp"
