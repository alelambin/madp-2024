#include <Windows.h>
#include <WinBase.h>
#include "manager.h"
#include "logger.h"

#define INI_FILE_NAME "./data.ini"
#define INI_SECTION_NAME "data"
#define INI_PRODUCER_PARAMETER "n"
#define INI_CONSUMER_PARAMETER "m"
#define INI_BROKER_PARAMETER "k"
#define INI_POOL_CAPACITY_PARAMETER "pool_capacity"
#define INI_DEFAULT_INT_VALUE -1

int main() {
    int producersCount = GetPrivateProfileInt(INI_SECTION_NAME, INI_PRODUCER_PARAMETER, INI_DEFAULT_INT_VALUE, INI_FILE_NAME);
    if (producersCount == INI_DEFAULT_INT_VALUE) {
        ERROR("Could not find parameter \"" + INI_PRODUCER_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\"\n");
        return -1;
    }
    int consumersCount = GetPrivateProfileInt(INI_SECTION_NAME, INI_CONSUMER_PARAMETER, INI_DEFAULT_INT_VALUE, INI_FILE_NAME);
    if (consumersCount == INI_DEFAULT_INT_VALUE) {
        ERROR("Could not find parameter \"" + INI_CONSUMER_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\"\n");
        return -1;
    }
    int brokersCount = GetPrivateProfileInt(INI_SECTION_NAME, INI_BROKER_PARAMETER, INI_DEFAULT_INT_VALUE, INI_FILE_NAME);
    if (brokersCount == INI_DEFAULT_INT_VALUE) {
        ERROR("Could not find parameter \"" + INI_BROKER_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\"\n");
        return -1;
    }
    int poolCapacity = GetPrivateProfileInt(INI_SECTION_NAME, INI_POOL_CAPACITY_PARAMETER, INI_DEFAULT_INT_VALUE, INI_FILE_NAME);
    if (poolCapacity == INI_DEFAULT_INT_VALUE) {
        ERROR("Could not find parameter \"" + INI_POOL_CAPACITY_PARAMETER + "\" in section \"" + INI_SECTION_NAME + "\"\n");
        return -1;
    }

    Manager manager(producersCount, consumersCount, brokersCount, poolCapacity);
    manager.run();
    while (!getchar());
    manager.end();

    return 0;
}
