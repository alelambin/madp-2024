#include "rand.h"

int random(const int min, const int max) {
    static std::default_random_engine gen(static_cast<unsigned>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(gen);
}
