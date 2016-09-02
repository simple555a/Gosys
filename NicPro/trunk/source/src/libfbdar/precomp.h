#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <librtk.h>
#include <f8.h>
#pragma hdrstop
#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <direct.h>
#include <string>
#include <map>

using namespace std;

#ifdef __LINUX__
#define PATH_SEPRATOR ":"
#elif defined(_WIN32)
#define PATH_SEPRATOR ";"
#else
#define PATH_SEPRATOR ":"
#endif

void blklib_load_devices();
void blklib_unload_devices();


