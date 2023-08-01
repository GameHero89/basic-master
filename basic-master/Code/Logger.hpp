#pragma once

class Logger
{
public:
	explicit Logger();
	~Logger() noexcept;

	Logger(Logger const&) = delete;
	Logger(Logger&&) = delete;
	Logger& operator=(Logger const&) = delete;
	Logger& operator=(Logger&&) = delete;

	void Info(const char* format, ...);

	void Error(const char* format, ...);

	void Custom(const char* type, const char* format, ...);

	void Log(const char* type, const char* format, std::va_list args = 0, bool file = true);

	std::mutex& GetMutex();
	std::pair<std::unique_ptr<char[]>*, std::size_t> GetMessages();
	std::istream& GetInput();
private:
	std::mutex m_Mutex;
	std::vector<std::unique_ptr<char[]>> m_Messages;

	fs::path m_FilePath;
	std::ofstream m_File;
	std::ofstream m_Console;
	std::ifstream m_Input;
};

inline Logger* g_Logger;

