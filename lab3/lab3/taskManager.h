#pragma once

#define CL_HPP_TARGET_OPENCL_VERSION 300
#define CL_HPP_ENABLE_EXCEPTIONS

#include <fstream>
#include <cstdint>
#include <string>
#include <vector>
#include <map>
#include <CL/cl.hpp>
#include "logger.h"

class TaskManager {
	int64_t minLimit;
	int64_t maxLimit;
	int threadsCount;
	cl::Kernel kernel;
	cl::CommandQueue queue;
	cl::Buffer buffer;
	size_t bufferSize;

public:
	TaskManager(int64_t minLimit, int64_t maxLimit, int threadsCount);
	~TaskManager() = default;
	std::map<int64_t, int64_t> execute();

};
