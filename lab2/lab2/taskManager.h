#pragma once

#include <cstdint>
#include <string>
#include <map>
#include <omp.h>
#include "task.h"
#include "logger.h"

class TaskManager {
	int64_t minLimit;
	int64_t maxLimit;
	int threadsCount;

public:
	TaskManager(int64_t minLimit, int64_t maxLimit, int threadsCount);
	~TaskManager() = default;
	std::map<int64_t, int64_t> execute();

};
