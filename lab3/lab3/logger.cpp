#include "logger.h"

void logger::print(std::string message) {
	print(message.c_str());
}

void logger::print(const char* message) {
	time_t now = time(0);
	tm tstruct;
	localtime_s(&tstruct, &now);
	char buffer[80];
	strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);

	std::cout << buffer << "  " << message << "\n";
}
