#include "taskManager.h"

TaskManager::TaskManager() {
	int procNum, procRank;
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	int64_t error = 0;
	try {
		if (procRank == MAIN_PROC_RANK) {
			std::string projectDirectory = EXPAND(PROJECT_DIR);
			projectDirectory.erase(0, 1);
			projectDirectory.erase(projectDirectory.rfind("."));
			std::string iniFileName = projectDirectory + INI_FILE_NAME;
			minLimit = parameters::getInt64(iniFileName.c_str(), INI_SECTION_NAME, INI_MIN_LIMIT_PARAMETER);
			maxLimit = parameters::getInt64(iniFileName.c_str(), INI_SECTION_NAME, INI_MAX_LIMIT_PARAMETER);
		}
	} catch(std::invalid_argument&) {
		error = 1;
	}

	int64_t buffer[3] = { error, minLimit, maxLimit };
	MPI_Bcast(buffer, 3, MPI_INT64_T, MAIN_PROC_RANK, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	error = buffer[0];
	minLimit = buffer[1];
	maxLimit = buffer[2];
	if (error) {
		throw std::invalid_argument("Invalid parameter");
	}

	if (minLimit < 1) {
		ERROR("Parameter \"" + std::string(INI_MIN_LIMIT_PARAMETER) + "\" in section \"" + std::string(INI_SECTION_NAME) + "\" is incorrect\n");
		throw std::invalid_argument("Invalid parameter");
	}
	if (maxLimit < minLimit) {
		ERROR("Parameter \"" + std::string(INI_MAX_LIMIT_PARAMETER) + "\" in section \"" + std::string(INI_SECTION_NAME) + "\" is incorrect\n");
		throw std::invalid_argument("Invalid parameter");
	}
}

std::map<int64_t, uint64_t> TaskManager::execute() {
	int procNum, procRank;
	MPI_Comm_size(MPI_COMM_WORLD, &procNum);
	MPI_Comm_rank(MPI_COMM_WORLD, &procRank);

	int64_t commonBufferSize = maxLimit - minLimit;
	commonBufferSize += (procNum - (commonBufferSize % procNum));
	int64_t procMinLimit = ((double)procRank / procNum) * commonBufferSize + minLimit;
	int64_t procMaxLimit = ((double)(procRank + 1) / procNum) * commonBufferSize + minLimit;
	if (procMaxLimit > maxLimit) {
		procMaxLimit = maxLimit;
	}
	int64_t procBufferSize = commonBufferSize / procNum;
	std::vector<Pair> procBuffer(procBufferSize, { 0, 0 });
	uint64_t index = 0;
	for (int64_t value = procMinLimit; value < procMaxLimit; value++) {
		procBuffer[index++] = { value, task::findSequenceLength(value) };
	}

	std::vector<Pair> commonBuffer(commonBufferSize, { 0, 0 });
	const int blockLengths[] = { 1, 1 };
	MPI_Aint displacesments[] = { 0, sizeof(int64_t) };
	MPI_Datatype types[] = { MPI_INT64_T, MPI_UINT64_T };
	MPI_Datatype type;
	MPI_Type_create_struct(2, blockLengths, displacesments, types, &type);
	MPI_Type_commit(&type);

	MPI_Gather(procBuffer.data(), procBufferSize, type, commonBuffer.data(), procBufferSize, type, MAIN_PROC_RANK, MPI_COMM_WORLD);
	MPI_Barrier(MPI_COMM_WORLD);

	std::map<int64_t, uint64_t> result;
	if (procRank == MAIN_PROC_RANK) {
		for (auto& data : commonBuffer) {
			int64_t value = data.value;
			uint64_t length = data.length;

			if (value == 0) {
				continue;
			}

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
