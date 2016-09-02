#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <f8.h>
#include <string>
#include <librtk.h>
using namespace std;

#pragma comment (lib, "librtk.lib")
#pragma comment (lib, "libfbdar.lib")
#pragma comment (lib, "libfcc.lib")

void _addGccPath()
{
	char buf[1024*16];
	char gccPath[F8_MAX_PATH];
	GetEnvironmentVariable("PATH", buf, sizeof(buf));
	string path = buf;
	sprintf(gccPath, "%sf8/x86/host", get_exe_dir());
	path = path + ";" + gccPath;
	SetEnvironmentVariable("PATH", path.c_str());
}

int main(int c, char **v)
{
	_addGccPath();
	init_blklib();
	return fcc_main_v(c, v);
}
