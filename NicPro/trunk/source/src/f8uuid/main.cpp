/*
	field bus controller simulator
	jackie, jhunter@tsinghua.org.cn
	2003/3/24
*/
#include <utils.h>
#include <f8.h>

#pragma comment (lib, "libutils.lib")
#pragma comment (lib, "libCPres.lib")

/*
*/
int main(int c, char **v)
{
	char	buf[128];
	__bool	bClipBoard=__false;
	f8_uuid	uuid;

	BEGIN_ARGS
		ARG("-c", bClipBoard, "tbool", "copy result to clipboard")
	END_ARGS

	if(process_args_1(c, v)){
		print_usage_1();
		return -1;
	}

	create_f8_uuid(&uuid);
	f8_uuid_to_string(&uuid, buf, sizeof(buf));
	printf("%s\n", buf);

#ifdef _WIN32
	//if(bClipBoard)
	{
		if( !OpenClipboard(NULL) ){
			return -1;
		}
		EmptyClipboard();
		HANDLE h = GlobalAlloc(GMEM_MOVEABLE, strlen(buf) + 1);
		char * p = (char *)GlobalLock(h);
		if(p){
			GlobalUnlock(h);
			strcpy(p, buf);
			SetClipboardData(CF_TEXT, h);
		}
		CloseClipboard();
	}
#endif

	return 0;
}
