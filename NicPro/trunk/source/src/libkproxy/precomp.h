#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <math.h>

#include <f8.h>
#include <f8_kproxy.h>
#include <ctype.h>

#ifdef _MSC_VER
#pragma warning(disable : 4786)
#endif

#include <shell.h>
#include <string>
#include <rtk_object.h>
#include <list>

using namespace std;

int on_exit(char *, void *);
int on_help(char *, void *k);

/*
	project commands
*/
int on_open(char *, void *);
int on_close(char *, void *);
int on_create(char *, void *);
int on_save(char *, void *);
int on_online(char *v, void *k);
int on_offline(char *v, void *k);
int on_uload(char *v, void *k);
int on_dload(char *v, void *k);
int on_fix(char *v, void *k);
int autoSave(char*,void*);

/*
	configuration management commands
*/
int on_ver(char *v, void *k);

/*
	block commands
*/
int on_mount(char *,void *);
int on_umount(char *,void *);
int on_mv(char *v, void *k);
int on_cp(char *v, void *k);
int on_entlib(char *v, void *k);
int on_cd(char *v, void *k);

/*
	task commands
*/
int on_reset(char *,void *);
int on_start(char *,void *k);
int on_stop(char *,void *k);
int on_break(char *,void *k);
int on_continue(char *,void *k);
int on_suspend(char *v, void * k);
int on_resume(char *v, void *k);

/*
	link commands
*/
int on_link(char *, void *k);
int on_ulink(char *, void *k);
int on_sort(char *v, void *k);

/*
	pin commands
*/
int on_exp(char *, void *k);
int on_uexp(char *, void *k);
int on_bind(char *, void *k);
int on_invert(char *, void *k);
int on_ubind(char *v, void *k);

/*
	diagnostic commands
*/
int on_ls(char *,void *k);
int on_print(char *,void *k);
int on_pp(char *, void *);
int on_disasm(char *v, void *k);
int on_error(char *v, void *k);
int on_mmap(char *v, void *k);
int on_dump(char *v, void *k);
int on_dump_res(char *v, void *k);
int on_whouses(char *v, void * k);
int on_whoowns(char *v, void *k);
int on_log(char *v, void *k);

/*
	variable management
*/
int on_lv(char *v, void * k);
int on_set(char *v, void * k);
int on_uset(char *v, void * k);
int on_modify(char *v, void *k);

/*
*/
int on_device(char *v, void * k);
int on_station(char *v, void * k);
int on_memsize(char *v, void *k);
int on_memusage(char *v, void *k);

/*
	misc commands
*/
int on_play(char *v, void *k);
int on_place(char *v, void *k);
int on_export(char *v, void *k);

char * _errmsg(int err);
string _blk_path(IBlk * b);
string _pin_path(struct blk_pin_t * p);
int _duplicate_blk(IBlk * src, IBlk **b);
int _instanciate_blk(IBlk * b);
int _mount_blk(
	IBlk * peer, 
	IBlk * sb,	/* an instanciated block */
	const char * name,
	bool keepMemoryBindings,
	void * k
	);
int _link_pins(
	struct blk_pin_t * sp, 
	struct blk_pin_t * tp,
	struct cblk_link_t ** rlk
	);

#define FS " \n\t\r"

#define successful (cmd.retcode == F8_SUCCESS)

struct proxy_shell_param {
	__uint	timeout;
};


/*
	define standard local variables for shell procs
*/
#define _shproc_prolog_no_k_() \
	struct kern_conf_cmd cmd;\
	IMemStream ms;\
	IF8Stream *s;\
	\
	__create__(IMemStream, &ms);\
	s = __ucast__(IMemStream, IF8Stream, (&ms));\
	memset(&cmd, 0, sizeof(cmd));\
	cmd.ibuffer = x_buffer;\
	OLD_ID(&cmd) = sections->h.uuid;\
	IMemStream_bind(&ms, x_buffer, x_buf_len)

/*
	define standard local variables for shell procs
*/
#define _shproc_prolog_() \
	_shproc_prolog_no_k_();\
	struct proxy_shell_param * psp = (struct proxy_shell_param *)k

/*
	convert a readonly block into a read/write block
*/
class CRWConverter 
{
public:
	CRWConverter(IBlk *b);
	int convert();
private:
	static __bool _ei_proc(
		const char * key,
		const void * buf,
		int len,
		int context
		);
	void convert_blk(const char *key, const void * buf, int len);
	void convert_lnk(const char *key, const void * buf, int len);
	void convert_pin(const char *key, const void * buf, int len);

	IBlk * par_blk;
	ICBlk * par_cblk;
};

/*
	convert a read/write block into a read/only block
*/
class CROConverter 
{
public:
	CROConverter(IBlk *b);
	int convert();
private:
	static __bool _ei_proc(
		const char * key,
		const void * buf,
		int len,
		int context
		);
	static __bool _make_readonly_proc(ITreeNode * nd, __uint context);
	
	void make_readonly(IBlk * blk);
	void convert_blk(const char *key, const void * buf, int len);
	void convert_lnk(const char *key, const void * buf, int len);
	void convert_pin(const char *key, const void * buf, int len);

	IBlk *	par_blk;
	ICBlk * par_cblk;
	int		curAction;
	void	*curObj;
	int		curIndex;
};

#define TOK_NUMBER		257
#define TOK_HEX_NUMBER	258
#define TOK_REAL_NUMBER	259
#define TOK_ID			260
#define TOK_DATE_MS		262
#define TOK_DATE_S		263

extern char yy_buffer[];
extern int yy_pointer;
extern char		*x_buffer;
extern __uint		x_buf_len;
extern IKAdapter	*adapter;
extern __bool b_ShellExit;
extern char fileName[F8_MAX_PATH];
extern char  g_blkpaths[4096];
extern int yylex( void );
extern string g_sProjectPath;
extern IBlk * currBlk;
extern IBlk * sections;
extern __bool g_bDirty;

__uint _type_size(__u8 type);
const char * _type_name(int type);

struct address_usage{
	int  count_owners;
	void * owners[8];
};

typedef void * HREG_LIST;
void reset_reg_map();
__bool alloc_reg(void * owner, struct mem_addr_t * addr, int length, f8_uint magic);
void * get_address_owner(const struct mem_addr_t * addr, int length, f8_uint * magic);
void free_reg(struct mem_addr_t * addr, int length);
__bool is_valid_address(const struct mem_addr_t * addr);
void set_address_owner(
	void * owner, 
	struct mem_addr_t * addr, 
	int length, 
	f8_uint magic, 
	__bool charge
	);
f8_status alloc_blk_reg_list(IBlk * b, HREG_LIST*);
f8_status gather_blk_reg_list(IBlk * b, HREG_LIST*);
void commit_reg_list(HREG_LIST h, __bool bCharge);
void delete_blk_reg_list(HREG_LIST, bool delete_and_free);
__bool get_address_usage(const mem_addr_t * addr, int length, address_usage * usage);
__bool build_reg_map();

f8_status upload_var_table();
f8_status download_var_table();
f8_status load_variables();
void unload_variables();
f8_status save_variables();
f8_status define_variable(const struct blk_var_t * v, struct blk_var_t ** _lv);
f8_status undefine_variable(struct blk_var_t * v);
f8_status delete_variable(struct blk_var_t * var);

void load_config();
void save_config();

f8_status check_program();

void init_clipboard();

int _ctrller_command(char * v, void * k, __u16 _cmd);

#define start_ctrller() _ctrller_command(0, 0, F8_START)
#define stop_ctrller() _ctrller_command(0, 0, F8_STOP)
#define reset_ctrller() _ctrller_command(0, 0, F8_RESET)

extern void yyrestart( FILE *);

#include <rtknet.h>
#define RTK_NO_PACKET_STRUCTS
#include <rtk_packet.h>
extern HVBUS vbus;

extern f8_status download_var_table();

#include <pshpack1.h>
struct TPoint{
	int x;
	int y;
};

struct TLinkMetrics{
	int count;
	TPoint coord[32];
};
#include <poppack.h>



