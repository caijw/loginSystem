#ifndef UTIL_H
#define UTIL_H
#include <ctime>
#include <ratio>
#include <chrono>

static long long currentTimeMillis() {
    //struct timeval tv{};
    //gettimeofday(&tv, nullptr);
    //return ((unsigned long long) tv.tv_sec * 1000 + (unsigned long long) tv.tv_usec / 1000);
    std::chrono::time_point<std::chrono::system_clock, std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(
            std::chrono::system_clock::now());
    auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    auto timestamp = tmp.count();
    return timestamp;
}

static unsigned int currentTimeSecond() {
    return currentTimeMillis() / 1000;
}
#endif