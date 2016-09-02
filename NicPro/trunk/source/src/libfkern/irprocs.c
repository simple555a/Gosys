#include "precomp.h"
#pragma hdrstop
#include <math.h>
#include <stdlib.h>
#include <stdarg.h>

static int _foo_func()
{
	fprintf(stderr,"Invalid system call\n");
	return 0;
}

void init_services()
{
	int i;
	if(blk_services[0]){
		/* already initialized */
		return;
	}
	printf("Initializing kernel services...\n");

	for(i=0;i<256;i++)
		blk_services[i]=(BLK_SERVICE_PROC)_foo_func;

#define _set(n, func) blk_services[n] = (BLK_SERVICE_PROC)_irqh_##func
	_set(0, sched);
	_set(1, cancel);
	_set(2, abs);
	_set(3, acos);
	_set(4, asin);
	_set(5, atan);
	_set(6, atan2);
	_set(7, cos);
	_set(8, cosh);
	_set(9, exp);
	_set(10, fabs);
	_set(11, fmod);
	_set(12, log);
	_set(13, log10);
	_set(14, pow);
	_set(15, sin);
	_set(16, sinh);
	_set(17, tan);
	_set(18, tanh);
	_set(19, sqrt);
	_set(20, gettime);
	_set(21, year);
	_set(22, month);
	_set(23, day);
	_set(24, halt);
	_set(25, hour);
	_set(26, minute);
	_set(27, second);
	_set(28, fprecision);
	_set(29, rand);
	_set(30, io_mem);
	_set(31, x_mem);
	_set(32, dev_cfg);

	_set(100,id_of_var);
	_set(101,read_var);
	_set(102,write_var);
	_set(103,f8_sprintf);
	_set(104,f8_printf);
#undef _set
}

/*
	if timeout < 0, then it specifies an absolute time
	else it's an relative time
*/
int _irqh_sched(double timeout, int p, f8_u8 ttype)
{	
	if((f8_u16)p >= g_kernel.current_blk->h.n_pins){
		ki_halt(&g_kernel, F8_INVALID_PIN_ID);
	}
	return ki_sched(&g_kernel, timeout, g_kernel.current_bblk->pins + p, ttype);
}

f8_bool _irqh_cancel(int handle, int p)
{
	if((f8_u16)p >= g_kernel.current_blk->h.n_pins){
		ki_halt(&g_kernel, F8_INVALID_PIN_ID);
	}
	return ki_cancel(&g_kernel, handle, g_kernel.current_bblk->pins + p);
}

int _irqh_abs(int a)
{
	return abs(a);
}

double  _irqh_acos(double a)
{
	return acos(a);
}

double _irqh_asin(double a)
{
	return asin(a);
}

double  _irqh_atan(double a)
{
	return atan(a);
}

double  _irqh_atan2(double a, double b)
{
	return atan2(a, b);
}

double  _irqh_cos(double a)
{
	return cos(a);
}

double  _irqh_cosh(double a)
{
	return cosh(a);
}

double  _irqh_exp(double a)
{
	return exp(a);
}

double  _irqh_fabs(double a)
{
	return fabs(a);
}

double  _irqh_fmod(double a, double b)
{
	return fmod(a, b);
}

double  _irqh_log(double a)
{
	return log(a);
}

double  _irqh_log10(double a)
{
	return log10(a);
}

double  _irqh_pow(double a, double b)
{
	return pow(a, b);
}

double  _irqh_sin(double a)
{
	return sin(a);
}

double  _irqh_sinh(double a)
{
	return sinh(a);
}

double  _irqh_tan(double a)
{
	return tan(a);
}

double  _irqh_tanh(double a)
{
	return tanh(a);
}

double  _irqh_sqrt(double a)
{
	return sqrt(a);
}

/*
	get absolute time in local timezone.
	in terms of seconds since January 1st, 1601.
*/
double _irqh_gettime(void)
{
	ktime_t t;
	double d;

	t = ke_get_time();
	d = t / 1e7;

	return d;
}

int _irqh_year(double t)
{
	assert(0);
	return 0;
}

int _irqh_month(double t)
{
	assert(0);
	return 0;
}

int _irqh_day(double t)
{
	assert(0);
	return 0;
}

void _irqh_halt(f8_status code)
{
	ki_halt(&g_kernel, code);
}

int _irqh_hour(double t)
{
	assert(0);
	return 0;
}

int _irqh_minute(double t)
{
	assert(0);
	return 0;
}

int _irqh_second(double t)
{
	assert(0);
	return 0;
}

double _irqh_fprecision()
{
	return 1e-9;
}

int	_irqh_rand(int a)
{
	return (int)(rand() * 1.0 * a / RAND_MAX);
}

f8_byte * _irqh_io_mem(f8_u16 addr)
{
	if(addr >= IO_MAX_DEVICES){
		return 0;
	}
	return &g_kernel.io_memory[addr][0];
}

f8_byte * _irqh_x_mem(f8_byte n, f8_u16 addr)
{
	if(addr >= g_kernel.x_mem_sizes[n]){
		return 0;
	}
	return g_kernel.x_mem_pointers[n];
}

f8_byte * _irqh_dev_cfg(f8_u16 addr)
{
	if(addr >= IO_MAX_DEVICES){
		return 0;
	}
	return g_kernel.devices[addr].configData;
}

f8_uuid _irqh_id_of_var(const char * name)
{
	return ki_id_of_var(name);
}

f8_bool _irqh_read_var(f8_uuid id,void*p)
{
	struct blk_var_t * v;
	v=ki_var_of_id(id);
	if(!v)
		return f8_false;
	memcpy(p,_irqh_x_mem(v->addr.section,v->addr.addr),_type_size(v->type));
	return f8_true;
}

f8_bool _irqh_write_var(f8_uuid id,void*p)
{
	struct blk_var_t * v;
	v=ki_var_of_id(id);
	if(!v)
		return f8_false;
	memcpy(_irqh_x_mem(v->addr.section,v->addr.addr),p,_type_size(v->type));
	return f8_true;
}

int _irqh_f8_sprintf (char *buf,char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	return vsprintf(buf,fmt,args);
}

int _irqh_f8_printf (char *fmt,...)
{
	va_list args;
	va_start(args,fmt);
	return vprintf(fmt,args);
}


