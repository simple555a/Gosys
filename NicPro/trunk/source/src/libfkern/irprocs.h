/* 
	interrupt handlers

	2004/3/1 soycola

	the interrupt handles must be run in the context of a specific
	executive, i.e., no two executives can be run simutaneously because
	in interrupt routines there's no way to tell which is the current
	one.
*/
void init_services();

int _irqh_sched(double timeout, int pin, f8_u8 ttype);
f8_bool _irqh_cancel(int handle, int pin);

int _irqh_abs(int);
double  _irqh_acos(double);
double  _irqh_asin(double);
double  _irqh_atan(double);
double  _irqh_atan2(double, double);
double  _irqh_cos(double);
double  _irqh_cosh(double);
double  _irqh_exp(double);
double  _irqh_fabs(double);
double  _irqh_fmod(double, double);
double  _irqh_log(double);
double  _irqh_log10(double);
double  _irqh_pow(double, double);
double  _irqh_sin(double);
double  _irqh_sinh(double);
double  _irqh_tan(double);
double  _irqh_tanh(double);
double  _irqh_sqrt(double); 
int _irqh_rand(int);

double _irqh_gettime(void);

int _irqh_year(double);
int _irqh_month(double);
int _irqh_day(double);

void _irqh_halt(f8_status);

int _irqh_hour(double);
int _irqh_minute(double);
int _irqh_second(double);

double _irqh_fprecision();

f8_byte * _irqh_io_mem(f8_u16 addr);
f8_byte * _irqh_x_mem(f8_byte n, f8_u16 addr);
f8_byte * _irqh_dev_cfg(f8_u16 addr);

f8_uuid _irqh_id_of_var(const char * name);
f8_bool _irqh_read_var(f8_uuid id,void *p);
f8_bool _irqh_write_var(f8_uuid id,void*p);

int _irqh_f8_sprintf (char *buf,char *fmt,...);
int _irqh_f8_printf (char *fmt,...);

