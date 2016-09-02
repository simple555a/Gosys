/*
	kernel internal procedure declarations
*/

void ki_start(struct kernel_t * k);
void ki_start_1();
void ki_stop(struct kernel_t * k);
void ki_halt(struct kernel_t * kernel, f8_status code);
void ki_log(struct kernel_t * kernel, f8_event code, int c1, int c2, int c3);
void ki_logblk(struct kernel_t * kernel, f8_event code, IBlk * b, int c3);
void ki_clear_user_program(struct kernel_t * kernel);
void ki_reset(struct kernel_t * kernel);

void ki_exec_algorithm(IBBlk * b, struct blk_func_t * f);

void ki_bugcheck(struct kernel_t * kernel, int c1, int c2, int c3, int c4);
f8_status ki_switch_to(f8_u8 mode);
f8_u8 ki_get_state();
void ki_update_flags();
void ki_load_default_config(struct kernel_t * kernel);
void ki_init_mem_pointers(struct kernel_t *kernel);

#ifdef _WIN32
#define F8_PROG_FILE	"f8prog.dat"
#define F8_CFG_FILE 	"f8cfg.dat"
#define F8_VARTAB_FILE "f8var.dat"
#define F8_VOLATILE_FILE "f8-vola.dat"
#elif defined(__VXWORKS__)
#define F8_PROG_FILE	"/e/f8prog.dat"
#define F8_CFG_FILE 	"/e/f8cfg.dat"
#define F8_VARTAB_FILE "/e/f8var.dat"
#define F8_VOLATILE_FILE "/e/f8-vola.dat"
#elif defined(__LINUX__)
#define F8_PROG_FILE	"/f8prog.dat"
#define F8_CFG_FILE 	"/f8cfg.dat"
#define F8_VARTAB_FILE "/f8var.dat"
#define F8_VOLATILE_FILE "/f8-vola.dat"
#endif

f8_status ki_save_nvram(HF8_KERNEL kernel);
f8_status ki_load_nvram(HF8_KERNEL kernel);
f8_status ki_save_volatile(struct kernel_t * kernel);
__bool ki_load_volatile(struct kernel_t * kernel);
f8_status ki_suspend_to_nvram(struct kernel_t * kernel);
f8_status ki_resume_from_nvram(struct kernel_t * kernel);

/* controller side variable table support */
void ki_init_var_table();
void ki_uninit_var_table();
void ki_reset_var_table();
f8_status ki_add_var(struct blk_var_t * v);
f8_status ki_remove_var(const f8_uuid * id);
f8_status ki_edit_var(struct blk_var_t * v);
f8_status ki_load_var_table(f8_uint count, struct blk_var_t * defs);
f8_status ki_save_var_table(IF8Stream * s);
f8_uuid ki_id_of_var(const char * name);
struct blk_var_t * ki_var_of_id(f8_uuid id);
void ki_load_variable_values();

/*IEC 61131-3 FBD support */
void ki_init_bblk(struct kernel_t * kernel, IBBlk * bb);
void ki_finalize_bblk(struct kernel_t * kernel, IBBlk * bb);
void fbd_start(struct kernel_t * kernel);
void fbd_stop(struct kernel_t * kernel);
__bool fbd_marshel_i_memory(struct kernel_t * kernel);
__bool fbd_unmarshel_i_memory(struct kernel_t * kernel);
void ki_exec_fbd();

/* IEC 61499 support */
int ki_alloc_timer(struct kernel_t * kernel);
int ki_alloc_timer_must_succeed(struct kernel_t * kernel);
int ki_free_timer(struct kernel_t * kernel, struct blk_timer_t *);
f8_status ki_marshel_timers_events(struct kernel_t * kernel);
__bool ki_unmarshel_timers_events(struct kernel_t * kernel);
int ki_sched(struct kernel_t * kernel, double timeout, struct blk_pin_t * p, f8_u8 ttype);
f8_bool ki_cancel(struct kernel_t * kernel, int handle, struct blk_pin_t * p);
__bool ki_eq(struct kernel_t * kernel, struct blk_pin_t * p);
struct blk_pin_t * ki_dq(struct kernel_t * kernel);
#define _get_timer(kernel, tid)	(&kernel->timers[tid])
void ki_timer_expires();
void ki_execute_event(struct blk_pin_t * p);
void ki_start_timers(struct kernel_t * kernel);
void ki_stop_timers(struct kernel_t * kernel);
void ki_exec_61499();

void ki_exec_ld();
void ki_exec_sfc();

#include <pshpack1.h>
/*
	auto-average support(low-pass filter)
*/
struct average_array_t{
	f8_r8 * buf;
	int len;
	int p;
	/* average value */
	f8_r8 a;
	/* last snapshot value */
	f8_r8 rt;
	/* min and max values, updated at realtime */
	f8_r8 m,M;
	/* allowable value range for the variable, snapshot values outside of this range will
	be discarded by the averager
	*/
	f8_r8 m1,M1;
}PACKED;
#include <poppack.h>

f8_r8 ki_average(struct average_array_t * a, f8_r8 v);
void ki_init_average(struct average_array_t * a, f8_r8 * arr, int len, f8_r8 M, f8_r8 m);

