#include "Common.hpp"

Logger::Logger()
{
	try
	{
		m_FilePath.append(std::getenv("appdata"));
		m_FilePath.append(BASE_NAME);
		if (!fs::exists(m_FilePath))
			fs::create_directory(m_FilePath);
		m_FilePath.append(BASE_NAME ".log");
	}
	catch (fs::filesystem_error const&)
	{
	}

	if (!AttachConsole(GetCurrentProcessId()))
		AllocConsole();
	SetConsoleTitleA(BASE_NAME);
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), FOREGROUND_BLUE | FOREGROUND_GREEN);
	SetConsoleCP(CP_UTF8);
	SetConsoleOutputCP(CP_UTF8);
	HANDLE hInput = GetStdHandle(STD_INPUT_HANDLE);
	DWORD prev_mode;
	GetConsoleMode(hInput, &prev_mode);
	SetConsoleMode(hInput, prev_mode & ENABLE_EXTENDED_FLAGS);

	m_Console.open("CONOUT$");
	m_Input.open("CONIN$");
	m_File.open(m_FilePath, std::ios_base::out | std::ios_base::app);

	m_Console << u8R"kek(    
    ____             _     
   / __ )____ ______(_)____
  / __  / __ `/ ___/ / ___/
 / /_/ / /_/ (__  ) / /__  
/_____/\__,_/____/_/\___/


)kek";
}

Logger::~Logger() noexcept
{
	FreeConsole();
}

void Logger::Info(const char* format, ...)
{
	std::va_list args{};
	va_start(args, format);
	Log("Info", format, args);
	va_end(args);
}

void Logger::Error(const char* format, ...)
{
	std::va_list args{};
	va_start(args, format);
	Log("Error", format, args);
	va_end(args);

	g_Running = false;
}

void Logger::Custom(const char* type, const char* format, ...)
{
	std::va_list args{};
	va_start(args, format);
	Log(type, format, args);
	va_end(args);
}

void Logger::Log(const char* type, const char* format, std::va_list args, bool file)
{
	std::lock_guard lock(m_Mutex);

	auto time = std::time(nullptr);
	auto tm = std::localtime(&time);

	char prefix[64] = {};
	std::snprintf(prefix, sizeof(prefix) - 1, "[%02d:%02d:%02d] [%s] ", tm->tm_hour, tm->tm_min, tm->tm_sec, type);

	std::size_t messageLength = std::vsnprintf(nullptr, 0, format, args) + 1;
	auto messageBuffer = std::make_unique<char[]>(messageLength);

	std::uninitialized_fill_n(messageBuffer.get(), messageLength, '\0');
	std::vsnprintf(messageBuffer.get(), messageLength, format, args);

	m_File << prefix << messageBuffer.get() << std::endl;
	m_Console << prefix << messageBuffer.get() << std::endl;

	m_Messages.push_back(std::move(messageBuffer));
}

std::mutex& Logger::GetMutex()
{
	return m_Mutex;
}

std::pair<std::unique_ptr<char[]>*, std::size_t> Logger::GetMessages()
{
	return std::make_pair(m_Messages.data(), m_Messages.size());
}

std::istream& Logger::GetInput()
{
	return m_Input;
}