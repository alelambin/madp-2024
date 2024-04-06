#pragma once

#include <cstdint>
#include <string>
#include <stdexcept>
#include "logger.h"

namespace parameters {
	std::string getString(const char* filename, const char* section, const char* parameterName, bool defaultValid = true);
	int64_t getInt64(const char* filename, const char* section, const char* parameterName);
	int getInt(const char* filename, const char* section, const char* parameterName);
};
