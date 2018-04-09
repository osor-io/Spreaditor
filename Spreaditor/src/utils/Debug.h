#pragma once
#include <iostream>
#include <sstream>

/* Usefull to avoid any LOGs in the console in release mode
#ifndef _DEBUG
#define _NOLOG
#endif
*/


#ifndef _NOLOG

#define ENDL std::cout << std::endl;

#define LOG(x) std::cout << x << std::endl

#define LOG_NAMED(x) std::cout << #x << ": " << x << std::endl

#define LOG_PTR(x) std::cout << #x << ": " << static_cast<void*>(x) << std::endl

#define LOG_ERROR(x) std::cerr << "ERROR: " << x << std::endl

#define CLOG(x) std::cout << x << std::endl; \
    {auto ss = std::stringstream{}; ss << x; log_to_file(ss.str().c_str());}

#define CLOG_NAMED(x) std::cout << #x << ": " << x << std::endl; \
    {auto ss = std::stringstream{}; ss << #x << ": " << x; log_to_file(ss.str().c_str());}

#define CLOG_PTR(x) \
  std::cout << #x << ": " << static_cast<void*>(x) << std::endl; \
    {auto ss = std::stringstream{}; ss << #x << ": " << static_cast<void*>(x); log_to_file(ss.str().c_str());}

#define CLOG_ERROR(x) std::cerr << "ERROR: " << x << std::endl; \
    {auto ss = std::stringstream{}; ss << "ERROR: " << x; log_to_file(ss.str().c_str());}

#else

#define ENDL
#define LOG(x)
#define LOG_NAMED(x)
#define LOG_PTR(x)
#define CLOG(x)
#define CLOG_NAMED(x)
#define CLOG_PTR(x)

#endif

void press_to_continue() noexcept;

void log_to_file(const char* msg);
