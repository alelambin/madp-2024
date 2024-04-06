#include "parameterManager.h"
#include <Windows.h>
#include <WinBase.h>

#define INI_MAX_PARAMETER_SIZE 160
#define INI_DEFAULT_STRING_VALUE ""

std::string parameters::getString(const char* filename, const char* section, const char* parameterName, bool defaultValid) {
    const int64_t parameterSize = INI_MAX_PARAMETER_SIZE;
    char* parameter = new char[parameterSize];
    GetPrivateProfileString(section, parameterName, INI_DEFAULT_STRING_VALUE, parameter, parameterSize, filename);
    if (!defaultValid && parameter[0] == '\0') {
        ERROR("Could not find parameter \"" + std::string(parameterName) + "\" in section \"" + std::string(section) + "\"\n");
        throw std::invalid_argument("Invalid parameter");
    }
    std::string result = parameter;
    delete parameter;
    return result;
}

int64_t parameters::getInt64(const char* filename, const char* section, const char* parameterName) {
    return std::stoll(getString(filename, section, parameterName, false));
}

int parameters::getInt(const char* filename, const char* section, const char* parameterName) {
    return std::stoi(getString(filename, section, parameterName, false));
}
