#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <map>
#include <mpi.h>
#include "taskManager.h"
#include "parameterManager.h"
#include "logger.h"

int main(int argc, char* argv[]) {
    MPI_Init(&argc, &argv);
    int procRank;
    MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

    double start, end;
    std::map<int64_t, uint64_t> result;
    try {
        TaskManager taskManager;
        start = MPI_Wtime();
        result = taskManager.execute();
        end = MPI_Wtime();
    } catch(...) {
        MPI_Finalize();
        return -1;
    }

    double procDuration = end - start;
    double duration;
    MPI_Reduce(&procDuration, &duration, 1, MPI_DOUBLE, MPI_MAX, MAIN_PROC_RANK, MPI_COMM_WORLD);
    MPI_Barrier(MPI_COMM_WORLD);

    if (procRank == MAIN_PROC_RANK) {
        std::string projectDirectory = EXPAND(PROJECT_DIR);
        projectDirectory.erase(0, 1);
        projectDirectory.erase(projectDirectory.rfind("."));
        std::string iniFileName = projectDirectory + INI_FILE_NAME;
        std::string filename = parameters::getString(iniFileName.c_str(), INI_SECTION_NAME, INI_OUTPUT_PARAMETER);
        if (filename.empty()) {
            for (auto& pair : result) {
                std::cout << pair.first << ": " << pair.second << "\n";
            }
        } else {
            std::ofstream file(projectDirectory + filename);
            if (file.is_open()) {
                for (auto& pair : result) {
                    file << pair.first << ": " << pair.second << "\n";
                }
            }
            file.close();
        }
        LOG("Execution time: " + std::to_string(duration) + " seconds");
    }

    MPI_Finalize();
    return 0;
}
