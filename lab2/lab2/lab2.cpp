#include <Windows.h>
#include <WinBase.h>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <string>
#include <map>
#include <chrono>
#include "taskManager.h"
#include "logger.h"

#define INI_FILE_NAME "./data.ini"
#define INI_SECTION_NAME "data"
#define INI_MIN_LIMIT_PARAMETER "n"
#define INI_MAX_LIMIT_PARAMETER "m"
#define INI_THREADS_COUNT_PARAMETER "threads_count"
#define INI_OUTPUT_PARAMETER "output"
#define INI_DEFAULT_STRING_VALUE ""
#define INI_GET_STRING(section, parameterName, string, stringSize, filename, defaultValid) {                    \
    GetPrivateProfileString(section, parameterName, INI_DEFAULT_STRING_VALUE, string, stringSize, filename);    \
    if (!defaultValid && parameter[0] == '\0') {                                                                \
        ERROR("Could not find parameter \"" + parameterName + "\" in section \"" + section + "\"\n");           \
        return -1;                                                                                              \
    }                                                                                                           \
}

int main() {
    const int64_t parameterSize = 160;
    char* parameter = new char[parameterSize];
    INI_GET_STRING(INI_SECTION_NAME, INI_MIN_LIMIT_PARAMETER, parameter, parameterSize, INI_FILE_NAME, false);
    int64_t minLimit = std::stoll(parameter);
    if (minLimit < 1) {
        ERROR("Parameter \"" + INI_MIN_LIMIT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }
    INI_GET_STRING(INI_SECTION_NAME, INI_MAX_LIMIT_PARAMETER, parameter, parameterSize, INI_FILE_NAME, false);
    int64_t maxLimit = std::stoll(parameter);
    if (maxLimit < minLimit) {
        ERROR("Parameter \"" + INI_MAX_LIMIT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }
    INI_GET_STRING(INI_SECTION_NAME, INI_THREADS_COUNT_PARAMETER, parameter, parameterSize, INI_FILE_NAME, false);
    int threadsCount = std::stoi(parameter);
    if (threadsCount < 1) {
        ERROR("Parameter \"" + INI_THREADS_COUNT_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\" is incorrect\n");
        return -1;
    }
    INI_GET_STRING(INI_SECTION_NAME, INI_OUTPUT_PARAMETER, parameter, parameterSize, INI_FILE_NAME, true);
    std::string filename = parameter;
    delete[] parameter;
    
    TaskManager taskManager(minLimit, maxLimit, threadsCount);
    auto start = std::chrono::high_resolution_clock::now();
    std::map<int64_t, int64_t> result = taskManager.execute();
    auto end = std::chrono::high_resolution_clock::now();
    if (filename.empty()) {
        for (auto& pair : result) {
            std::cout << pair.first << ": " << pair.second << "\n";
        }
    } else {
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
