#include "taskManager.h"

#define CL_FILE_NAME "./kernel.cl"
#define CL_MAIN_FUNCTION "find_sequence_length"

TaskManager::TaskManager(int64_t minLimit, int64_t maxLimit, int threadsCount)
	: minLimit(minLimit), maxLimit(maxLimit), threadsCount(threadsCount) {
	cl::Device device = cl::Device::getDefault();
	cl::Context context = cl::Context::getDefault();

	std::string line, source;
	std::ifstream file(CL_FILE_NAME);
	if (!file.is_open()) {
		ERROR("File " + CL_FILE_NAME + " could not be open");
		throw std::runtime_error("File error");
	}
	while (std::getline(file, line)) {
		source += line;
		source += '\n';
	}
	file.close();

	bufferSize = maxLimit - minLimit;
	std::vector<int64_t> values(bufferSize, 0);
	for (size_t i = 0; i < bufferSize; i++) {
		values[i] = minLimit + i;
	}
	buffer = cl::Buffer(context, CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, bufferSize * sizeof(int64_t), values.data());

	queue = cl::CommandQueue(context, device);
	cl::Program program(context, source);
	try {
		program.build(device);
	}
	catch (const cl::Error& exception) {
		ERROR(program.getBuildInfo<CL_PROGRAM_BUILD_LOG>(device));
		ERROR("Kernel source compilation error");
		throw std::runtime_error(exception.what());
	}

	kernel = cl::Kernel(program, CL_MAIN_FUNCTION);
	kernel.setArg(0, (int)bufferSize);
	kernel.setArg(1, buffer);
}

std::map<int64_t, int64_t> TaskManager::execute() {
	std::map<int64_t, int64_t> result;

	std::vector<int64_t> lengths(bufferSize, 0);
	queue.enqueueNDRangeKernel(kernel, cl::NullRange, bufferSize, cl::NullRange);
	queue.enqueueReadBuffer(buffer, CL_TRUE, 0, bufferSize * sizeof(int64_t), lengths.data());

	for (size_t i = 0; i < bufferSize; i++) {
		int64_t value = minLimit + i;
		int64_t length = lengths[i];

		DEBUG(std::to_string(value) + " has a sequence length of " + std::to_string(length));
		auto pair = result.find(length);
		if (pair != result.end()) {
			pair->second++;
		}
		else {
			result.insert({ length, 1 });
		}
	}
	return result;
}
