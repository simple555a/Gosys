#include <pmc.h>

#ifdef _WIN32
#include <direct.h>
#else
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#endif

#include <locale.h>
#include <string.h>
#include <string>

using namespace std;

#include <ntservice.h>

#ifdef _WIN32
#include <LimitSingleInstance.h>
#endif