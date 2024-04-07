#include "logger.h"

void logger::print(std::string message) {
	print(message.c_str());
}

void logger::print(const char* message) {
	int procRank;
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);
	if (procRank == MAIN_PROC_RANK) {
		time_t now = time(0);
		tm tstruct;
		localtime_s(&tstruct, &now);
		char buffer[80] = "\0";
		strftime(buffer, sizeof(buffer), "%Y-%m-%d %X", &tstruct);

		std::cout << buffer << "  " << message << "\n";
	}
}
