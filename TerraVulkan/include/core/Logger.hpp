#ifndef LOGGER_HPP
#define LOGGER_HPP

#define NOMINMAX
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <thread>
#include <iomanip>
#include <ctime>
#include <format>
#include <chrono>
#include <filesystem>
#include "util/ANSIFormatter.hpp"

#define Logger_Initialize() __LoggerInitialize()

#define Logger_WriteConsole(message, level) __LoggerWriteConsole(message, std::string(__FUNCTION__), level)
#define Logger_ThrowError(unexpectedd, message, fatal) __LoggerThrowError(unexpectedd, message, std::string(__FUNCTION__), __LINE__, fatal)

#define Logger_FunctionStart Logger_WriteConsole("Attempting to initalize " + std::string(__FUNCTION__) + "...", LogLevel::DEBUG)
#define Logger_FunctionEnd Logger_WriteConsole("Successfully initalized " + std::string(__FUNCTION__) + "!", LogLevel::DEBUG)

#define Logger_CleanUp() __LoggerCleanUp()

enum class LogLevel
{
	INFO,
	DEBUG,
	WARNING,
	ISSUE,
	FATAL_ERROR
};

using namespace std::chrono;

std::string loggerCurrentMessage;
std::string loggerSaveFilename;
std::ofstream loggerSaveFile;
std::thread::id loggerThreadID;
long long loggerTimeNow;
struct tm loggerLocalTime;

bool loggerClosing = false;

std::string GetTimeFormatted(const std::string& format)
{
	system_clock::time_point now = std::chrono::system_clock::now();
	time_t time = std::chrono::system_clock::to_time_t(now);

	std::tm snapshot;

#if defined(__unix__)
	localtime_r(&in_time_t, &tm_snapshot);
#elif defined(_MSC_VER)
	localtime_s(&snapshot, &time);
#else
	static std::mutex mtx;
	std::lock_guard<std::mutex> lock(mtx);
	tm_snapshot = *std::localtime(&in_time_t);
#endif

	std::stringstream timeStream;
	timeStream << std::put_time(&snapshot, format.c_str());

	return timeStream.str();
}

void __LoggerInitialize() 
{
	if (!std::filesystem::exists("logs"))
		std::filesystem::create_directories("logs");

	loggerSaveFilename = "log_" + GetTimeFormatted("%m_%d_%Y___%M_%S_%H") + ".log";
	loggerSaveFile.open("logs/" + loggerSaveFilename);

	if (!loggerSaveFile.is_open())
		std::cerr << "Failed to open or create the file!" << std::endl;
}

void __LoggerWriteConsole(const std::string& message, const std::string& function, LogLevel level)
{
	int position = 0;
	std::string name;

	while (function[position] != ':' && position < function.size())
	{
		name += function[position];

		position++;
	}

	name[0] = toupper(name[0]);
	loggerThreadID = std::this_thread::get_id();

	std::stringstream threadSS;
	threadSS << loggerThreadID;

	std::string time = GetTimeFormatted("%M:%S:%H");

	switch (level)
	{
	case LogLevel::INFO:
		loggerCurrentMessage = ANSIFormatter::format("&2[{}&2] [Thread ({})/INFO] [{}&2]: {}&2&r", time.c_str(), threadSS.str().c_str(), name.c_str(), message.c_str());
		std::cout << loggerCurrentMessage << std::endl;
		loggerSaveFile << ANSIFormatter::deFormat(loggerCurrentMessage) + "\n";
		loggerCurrentMessage = "";
		break;

	case LogLevel::DEBUG:

		loggerCurrentMessage = ANSIFormatter::format("&1[{}&1] [Thread ({})/DEBUG] [{}&1]: {}&1&r", time.c_str(), threadSS.str().c_str(), name.c_str(), message.c_str());
		std::cout << loggerCurrentMessage << std::endl;
		loggerSaveFile << ANSIFormatter::deFormat(loggerCurrentMessage) + "\n";
		loggerCurrentMessage = "";
		break;

	case LogLevel::WARNING:
		loggerCurrentMessage = ANSIFormatter::format("&6[{}&6] [Thread ({})/WARNING] [{}&6]: {}&6&r", time.c_str(), threadSS.str().c_str(), name.c_str(), message.c_str());
		std::cout << loggerCurrentMessage << std::endl;
		loggerSaveFile << ANSIFormatter::deFormat(loggerCurrentMessage) + "\n";
		loggerCurrentMessage = "";
		break;

	case LogLevel::ISSUE:
		loggerCurrentMessage = ANSIFormatter::format("&4[{}&4] [Thread ({})/ERROR] [{}&4]: {}&4&r", time.c_str(), threadSS.str().c_str(), name.c_str(), message.c_str());
		std::cout << loggerCurrentMessage << std::endl;
		loggerSaveFile << ANSIFormatter::deFormat(loggerCurrentMessage) + "\n";
		loggerCurrentMessage = "";
		break;

	case LogLevel::FATAL_ERROR:
		loggerCurrentMessage = ANSIFormatter::format("&4[{}&4] [Thread ({})/FATAL ERROR] [{}&4]: {}&4&r", time.c_str(), threadSS.str().c_str(), name.c_str(), message.c_str());
		std::cout << loggerCurrentMessage << std::endl;
		loggerSaveFile << ANSIFormatter::deFormat(loggerCurrentMessage) + "\n";
		loggerCurrentMessage = "";
		break;

	default:
		break;
	}
}

void __LoggerCleanUp();

void __LoggerThrowError(const std::string& unexpected, const std::string& message, const std::string& function, int line, bool fatal)
{
	int position = 0;
	std::string name;

	while (function[position] != ':' && position < function.size())
	{
		name += function[position];

		position++;
	}

	name[0] = toupper(name[0]);

	if (!fatal)
		__LoggerWriteConsole(std::format("Unexpected: '{}' at: '{}::{}', {}", unexpected.c_str(), name.c_str(), line, message.c_str()), function, LogLevel::ISSUE);
	else
	{
		__LoggerWriteConsole(std::format("Unexpected: '{}' at: '{}::{}', {}", unexpected.c_str(), name.c_str(), line, message.c_str()), function, LogLevel::FATAL_ERROR);

		MessageBoxA(NULL, std::format("Unexpected: '{}' at: '{}::{}', {}", unexpected.c_str(), name.c_str(), line, message.c_str()).c_str(), "Fatal Error!", MB_ICONERROR);
		
		PostQuitMessage(-1);

		Logger_CleanUp();
	}
}

void __LoggerCleanUp()
{
	loggerSaveFile.close();
}

#endif