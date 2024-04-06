#pragma once

#include <iostream>
#include <sstream>
#include <string>
#include <mutex>

#define LOG(...) {                                        \
	logger::print(std::string("[INFO] ") + __VA_ARGS__);  \
}
#define ERROR(...) {                                      \
	logger::print(std::string("[ERROR] ") + __VA_ARGS__); \
}
#ifdef _DEBUG
#define DEBUG(...) {                                      \
	logger::print(std::string("[DEBUG] ") + __VA_ARGS__); \
}
#else
#define DEBUG(...)
#endif

namespace logger {
	void print(std::string message);
	void print(const char* message);
};
