#include <iostream>
#include <chrono>
#include <iomanip>
#include <cstdlib>  // For std::exit

namespace traceUtil
{
// Global variable to track active trace groups
inline int activeTraceGroups = 0;  // Bitmask for trace groups

// Global variable to store the last print time
inline auto lastPrintTime = std::chrono::steady_clock::now();

// Function to get current time in 24-hour format as a string
std::string currentTime()
{
    auto now = std::chrono::system_clock::now();
    std::time_t nowTime = std::chrono::system_clock::to_time_t(now);
    std::tm localTime = *std::localtime(&nowTime);

    std::ostringstream oss;
    oss << std::put_time(&localTime, "%H:%M:%S");
    return oss.str();
}

// Function to calculate and return the elapsed time since last print
double elapsedTimeSinceLastPrint()
{
    auto now = std::chrono::steady_clock::now();
    double elapsed = std::chrono::duration<double>(now - lastPrintTime).count();
    lastPrintTime = now;  // Update last print time to current
    return elapsed;
}

// INFO Macro: Prints message with time and elapsed time
#define INFO(msg) \
        std::cerr << "[INFO] " << traceUtil::currentTime() << " (" << std::fixed << std::setprecision(3) \
                  << traceUtil::elapsedTimeSinceLastPrint() << "s since last) (" << __FILE__ << ":" << __LINE__ << "): " << msg << std::endl

// ERROR Macro: Prints message, then terminates the program
#define ERROR(msg) \
        do { \
            std::cerr << "[ERROR] " << traceUtil::currentTime() << " (" << std::fixed << std::setprecision(3) \
                      << traceUtil::elapsedTimeSinceLastPrint() << "s since last) (" << __FILE__ << ":" << __LINE__ << "): " << msg << std::endl; \
            std::exit(EXIT_FAILURE); \
        } while (0)

// TRACE Macro: Prints message if specified trace group is active
#define TRACE(group, msg) \
        do { \
            if (traceUtil::activeTraceGroups & (1 << group)) { \
                std::cerr << "[TRACE " << group << "] " << traceUtil::currentTime() << " (" << std::fixed << std::setprecision(3) \
                          << traceUtil::elapsedTimeSinceLastPrint() << "s since last) (" << __FILE__ << ":" << __LINE__ << "): " << msg << std::endl; \
            } \
        } while (0)
}

