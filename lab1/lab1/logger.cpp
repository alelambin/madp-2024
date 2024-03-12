#include "logger.h"

Logger* Logger::instance = nullptr;

Logger* Logger::getInstance() {
	if (!instance) {
		instance = new Logger();
	}
	return instance;
}

Logger::~Logger() {
	delete instance;
}

void Logger::print(std::string message) {
	print(message.c_str());
}

void Logger::print(const char* message) {
	mutex.lock();

	time_t now = time(0);
	tm tstruct;
	localtime_s(&tstruct, &now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);

	std::cout << buffer << "  " << message << "\n";

	mutex.unlock();
}

std::string str(double value) {
	return std::to_string(value);
}

std::string str(long double value) {
	return std::to_string(value);
}

std::string str(float value) {
	return std::to_string(value);
}

std::string str(long long value) {
	return std::to_string(value);
}

std::string str(unsigned long long value) {
	return std::to_string(value);
}

std::string str(long value) {
	return std::to_string(value);
}

std::string str(unsigned long value) {
	return std::to_string(value);
}

std::string str(int value) {
	return std::to_string(value);
}

std::string str(unsigned int value) {
	return std::to_string(value);
}

std::string str(Message value) {
	return "Message \"" + value.data + "\" to " + std::to_string(value.targetId);
}
