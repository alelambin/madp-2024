#include "taskManager.h"

TaskManager::TaskManager(int64_t minLimit, int64_t maxLimit, int threadsCount)
	: minLimit(minLimit), maxLimit(maxLimit), threadsCount(threadsCount) {
	omp_set_num_threads(threadsCount);
}

std::map<int64_t, int64_t> TaskManager::execute() {
	std::map<int64_t, int64_t> result;
	
#pragma omp parallel for
	for (int64_t value = minLimit; value < maxLimit; value++) {
		uint64_t length = task::findSequenceLength(value);

#pragma omp critical
		{
			DEBUG(std::to_string(value) + " has a sequence length of " + std::to_string(length));
			auto pair = result.find(length);
			if (pair != result.end()) {
				pair->second++;
			}
			else {
				result.insert({ length, 1 });
			}
		}
	}
	return result;
}
