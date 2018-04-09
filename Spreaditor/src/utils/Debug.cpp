#include "Debug.h"
#include "../debug/LoggingManager.h"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32) && !defined(__CYGWIN__)
void press_to_continue() noexcept { system("pause"); }
#else
void press_to_continue() noexcept { system("read"); }
#endif


void log_to_file(const char* msg) {
    LOGF(msg);
}