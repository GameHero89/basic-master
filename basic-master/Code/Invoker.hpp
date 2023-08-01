#pragma once

namespace rage
{
	class scrNativeCallContext
	{
	public:
		void Reset()
		{
			m_ArgCount = 0;
			m_DataCount = 0;
		}
		template <typename T>
		void PushArg(T&& value) {
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			*reinterpret_cast<std::remove_cv_t<std::remove_reference_t<T>>*>(reinterpret_cast<std::uint64_t*>(m_Args) + (m_ArgCount++)) = std::forward<T>(value);
		}
		template <typename T>
		T& GetArg(std::size_t index) {
			static_assert(sizeof(T) <= sizeof(std::uint64_t));
			return *reinterpret_cast<T*>(reinterpret_cast<std::uint64_t*>(m_Args) + index);
		}
		template <typename T>
		T* GetReturnValue() {
			return reinterpret_cast<T*>(m_ReturnValue);
		}
	protected:
		void* m_ReturnValue;
		std::uint32_t m_ArgCount;
		void* m_Args;
		std::int32_t m_DataCount;
		std::uint32_t m_Data[48];
	};

	using scrNativeHash = std::uint64_t;
	using scrNativeMapping = std::pair<scrNativeHash, scrNativeHash>;
	class scrNativeRegistrationTable
	{
		void* m_entries[0xFF];
		std::uint32_t m_unk;
		bool m_initialized;
	};

	static_assert(sizeof(scrNativeCallContext) == 0xE0);
}

class CallContext : public rage::scrNativeCallContext
{
public:
	CallContext() {
		m_ReturnValue = &m_ReturnStack[0];
		m_Args = &m_ArgStack[0];
	}
private:
	std::uint64_t m_ReturnStack[10];
	std::uint64_t m_ArgStack[100];
};

class Invoker
{
public:
	Invoker();
	~Invoker() {
		m_HandlerCache.clear();
	}
	void BeginCall() {
		m_CallContext.Reset();
	}

	void* GetHandler(rage::scrNativeHash hash);
	void EndCall(rage::scrNativeHash hash);

	template <typename T>
	void PushArg(T&& value)
	{
		m_CallContext.PushArg(std::forward<T>(value));
	}

	template <typename T>
	T& GetReturnValue()
	{
		return *m_CallContext.GetReturnValue<T>();
	}

	std::unordered_map<rage::scrNativeHash, void*> m_HandlerCache;
private:
	CallContext m_CallContext;
};

inline Invoker* g_Invoker;

template <typename Ret, typename ...Args>
FORCEINLINE Ret invoke(rage::scrNativeHash hash, Args &&...args)
{
	g_Invoker->BeginCall();
	(g_Invoker->PushArg(std::forward<Args>(args)), ...);
	g_Invoker->EndCall(hash);

	if (!std::is_same_v<Ret, void>) {
		return g_Invoker->GetReturnValue<Ret>();
	}
}
