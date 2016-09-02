#include<utils.h>
#include<f8.h>



#pragma comment(lib,"libkserver.lib")
#pragma comment(lib,"libkadapter.lib")
#pragma comment(lib,"libkproxy.lib")
#pragma comment(lib,"libfkern.lib")
#pragma comment(lib,"libfbd.lib")
#pragma comment(lib,"libfbdar.lib")
#pragma comment(lib,"libCPres.lib")
#pragma comment(lib,"libCPmm.lib")
#pragma comment(lib,"libcobj.lib")
#pragma comment(lib,"libutils.lib")

static int count;


int main(int c,char ** v)
{
	__bool bSet1131 = __false;
	char *ifname = 0;
	IBlk * b;
	BEGIN_ARGS
		ARG("-if:",ifname,"char *","input file name")
		ARG("-set:1131",bSet1131,"tbool","set the auto scan flag")
	END_ARGS
	if(process_args_1(c,v) < 0)
	{
		print_usage_1();
		return -1;
	}

	if(!ifname)
	{
		print_usage_1();
		return -1;
	}
	blklib_load_by_file(ifname,&b);
	if(!b)
	{
		utils_error("Error opening %s \n",ifname);
		return -1;
	}

	if(bSet1131)
	{
		b->h.flags |= BLK_F_AUTOSCAN;
		blk_to_file(b,ifname);
	}
	
	delete_blk(b);
}