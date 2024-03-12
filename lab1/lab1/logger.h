#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>
#include "message.h"

#define LOG(...) {                                        \
	Logger::print(std::string("[INFO] ") + __VA_ARGS__);  \
}
#define ERROR(...) {                                      \
	Logger::print(std::string("[ERROR] ") + __VA_ARGS__); \
}
#ifdef _DEBUG
#define DEBUG(...) {                                      \
	Logger::print(std::string("[DEBUG] ") + __VA_ARGS__); \
}
#else
#define DEBUG(...)
#endif

class Logger {
	static std::mutex mutex;

public:
	static void print(std::string message);
	static void print(const char *message);

};

std::string str(double value);
std::string str(long double value);
std::string str(float value);
std::string str(long long value);
std::string str(unsigned long long value);
std::string str(long value);
std::string str(unsigned long value);
std::string str(int value);
std::string str(unsigned int value);
std::string str(Message value);
