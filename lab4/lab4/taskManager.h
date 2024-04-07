#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <mpi.h>
#include "task.h"
#include "logger.h"
#include "parameterManager.h"

#define INI_FILE_NAME "data.ini"
#define INI_SECTION_NAME "data"
#define INI_MIN_LIMIT_PARAMETER "n"
#define INI_MAX_LIMIT_PARAMETER "m"
#define INI_OUTPUT_PARAMETER "output"

#define STRINGIFY(x) #x
#define EXPAND(x) STRINGIFY(x)

struct Pair {
	int64_t value;
	uint64_t length;
};

class TaskManager {
	int64_t minLimit;
	int64_t maxLimit;

public:
	TaskManager();
	~TaskManager() = default;
	std::map<int64_t, uint64_t> execute();

};
