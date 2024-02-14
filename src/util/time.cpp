
#ifdef _WIN32
#include <windows.h>
#else
#include <sys/time.h>
#include <unistd.h>
#endif

#include "time.hpp"

using namespace sim::util;


unsigned long time::get_now()
{
#ifdef _WIN32
	FILETIME ft;
    ULARGE_INTEGER ul;
    GetSystemTimeAsFileTime(&ft);
    ul.LowPart = ft.dwLowDateTime;
    ul.HighPart = ft.dwHighDateTime;
    return (unsigned long)(ul.QuadPart / 10 - 11644473600000000ULL);
#else
	struct timeval tv;
	gettimeofday(&tv, nullptr);
	return (unsigned long)tv.tv_sec * 1000000 + tv.tv_usec;
#endif
}

void time::sleep(unsigned long usec)
{
#ifdef _WIN32
	Sleep(usec / 1000);
#else
	usleep(usec);
#endif
}

