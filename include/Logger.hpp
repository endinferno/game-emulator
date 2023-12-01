#pragma once

#include "fmt/format.h"

#define INFO_ENABLE 1
#ifdef NDEBUG
#    define DEBUG_ENABLE 0
#else
#    define DEBUG_ENABLE 1
#endif
#define ERROR_ENABLE 1

std::string ClassFuncName(std::string&& name);

#define __CLASS_FUNC__ ClassFuncName(std::string(__PRETTY_FUNCTION__))

#if INFO_ENABLE
#    define INFO(format, ...)                  \
        do {                                   \
            fmt::print(format, ##__VA_ARGS__); \
        } while (0)
#else
#    define INFO(format, ...) \
        do {                  \
        } while (0)
#endif

#if DEBUG_ENABLE
#    define DEBUG(format, ...)                               \
        do {                                                 \
            fmt::print("FUNC: {:<25}, LINE: {:<4}: " format, \
                       __CLASS_FUNC__,                       \
                       __LINE__,                             \
                       ##__VA_ARGS__);                       \
        } while (0)
#else
#    define DEBUG(format, ...) \
        do {                   \
        } while (0)
#endif

#if ERROR_ENABLE
#    define ERROR(format, ...)                               \
        do {                                                 \
            fmt::print("FUNC: {:<25}, LINE: {:<4}: " format, \
                       __CLASS_FUNC__,                       \
                       __LINE__,                             \
                       ##__VA_ARGS__);                       \
        } while (0)
#else
#    define ERROR(format, ...) \
        do {                   \
        } while (0)
#endif
