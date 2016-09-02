/*
	field bus controller simulator
	jackie, jhunter@tsinghua.org.cn
	2003/3/24
*/
#include <utils.h>
#include <f8.h>

#pragma comment (lib, "libfbdar.lib")
#pragma comment (lib, "libfbd.lib")
#pragma comment (lib, "libCPres.lib")
#pragma comment (lib, "libcobj.lib")
#pragma comment (lib, "libutils.lib")

static int count;
static FILE * fdb;

static int UTILS_API _e_file(
	const char * szBaseDir,
	const char * szFullPath,
	const char * szRelativePath,
	const void * pvSearchContext
	)
{
	IBlk * blk;
	char buf[128];
	IFileStream *fs;
	IF8Stream *s;
	int roffset;

	fs = IFileStream_from_file(szFullPath, "rb");
	if(!fs){
		return 1;
	}
	s = __ucast__(IFileStream, IF8Stream, (fs));
	blk = IBlk_from_stream(s);
	if(!blk){
		__delete__(fs);
		return 1;
	}
	roffset = __vcall__(s, tell, (s));
	load_res_stream(s);
	probe_blk_resources(blk);
	
	fprintf(fdb, 
		"%d=%s,%c%c,%s", 
		count,
		IBlk_class_name(blk),
		blk->h.magic >> 8,
		blk->h.magic & 0xff,
		szRelativePath
		);
	f8_uuid_to_string(&blk->h.uuid, buf, sizeof(buf));
	fprintf(fdb, ",%s", buf);
	fprintf(fdb, ",%d", roffset);
	fprintf(fdb, "\n");
	
	delete_blk(blk);
	__delete__(fs);

	count++;
	return 1;
}

/*
*/
int main(int c, char **v)
{
	char	buf[1024];
	CArgs	args;
	char	*dirName = ".";
	char	*catagory=0;
	char	path[F8_MAX_PATH];

	BEGIN_ARGS
		ARG("-d:", dirName, "char *", "specify operation directory.")
		ARG("-c:", catagory, "char *", "specify library catagory[dir name]")
	END_ARGS

	if(process_args_1(c, v)){
		print_usage_1();
		return -1;
	}
	
	if(!SetCurrentDirectory(dirName)){
		utils_error("Cannot change directory to ``%s''.\n", dirName);
		return -1;
	}
	GetCurrentDirectory(sizeof(buf), buf);
	parse_arg_ex(buf, &args, ":/\\");

	if(!catagory){
		catagory = args.argv[args.argc-1];
	}

	GetCurrentDirectory(sizeof(path), path);
	printf(
		"Making library database in ``%s'', catagory ``%s''\n",
		path,
		catagory
		);

	fdb = fopen("f8.dir", "w");
	if(!fdb){
		return -1;
	}
	fprintf(fdb, "[library]\n");
	fprintf(fdb, "catagory=%s\n", catagory);

	enumerate_files(
		".",
		"*.blk",
		_e_file,
		0,
		0);
	fprintf(fdb, "count=%d\n", count);
	fclose(fdb);

	printf("%d entries added.\n", count);
	
	return 0;
}
