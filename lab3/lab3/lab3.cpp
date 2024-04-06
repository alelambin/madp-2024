#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <map>
#include <chrono>
#include "taskManager.h"
#include "parameterManager.h"
#include "logger.h"

#define INI_FILE_NAME "./data.ini"
#define INI_SECTION_NAME "data"
#define INI_MIN_LIMIT_PARAMETER "n"
#define INI_MAX_LIMIT_PARAMETER "m"
#define INI_THREADS_COUNT_PARAMETER "threads_count"
#define INI_OUTPUT_PARAMETER "output"

int main() {
    int64_t minLimit, maxLimit;
    int threadsCount;
    std::string filename;
    try {
        minLimit = parameters::getInt64(INI_FILE_NAME, INI_SECTION_NAME, INI_MIN_LIMIT_PARAMETER);
        maxLimit = parameters::getInt64(INI_FILE_NAME, INI_SECTION_NAME, INI_MAX_LIMIT_PARAMETER);
        threadsCount = parameters::getInt(INI_FILE_NAME, INI_SECTION_NAME, INI_THREADS_COUNT_PARAMETER);
        filename = parameters::getString(INI_FILE_NAME, INI_SECTION_NAME, INI_OUTPUT_PARAMETER);
    } catch (std::invalid_argument&) {
        return -1;
    }

    if (minLimit < 1) {
        ERROR("Parameter \"" + INI_MIN_LIMIT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }
    if (maxLimit < minLimit) {
        ERROR("Parameter \"" + INI_MAX_LIMIT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }
    if (threadsCount < 1) {
        ERROR("Parameter \"" + INI_THREADS_COUNT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }

    std::chrono::high_resolution_clock::time_point start, end;
    std::map<int64_t, int64_t> result;
    try {
        TaskManager taskManager(minLimit, maxLimit, threadsCount);
        start = std::chrono::high_resolution_clock::now();
        result = taskManager.execute();
        end = std::chrono::high_resolution_clock::now();
    } catch(...) {
        return -1;
    }

    if (filename.empty()) {
        for (auto& pair : result) {
            std::cout << pair.first << ": " << pair.second << "\n";
        }
    }
    else {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (auto& pair : result) {
                file << pair.first << ": " << pair.second << "\n";
            }
        }
        file.close();
    }

    std::chrono::duration<double> duration = end - start;
    LOG("Execution time: " + std::to_string(duration.count()) + " seconds");
    return 0;
}
