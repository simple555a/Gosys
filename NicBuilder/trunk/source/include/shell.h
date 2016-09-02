/*
	PC based Monitoring System source file
	version 1.0
	by J.Hunter,jhunter@263.net,May 2001 
*/
#ifndef __RTK_SHELL__
#define __RTK_SHELL__

#include <string.h>
#include <utils.h>
#include <librtk.h>

#ifdef _WIN32
#ifndef LIBRTKSHELL_EXPORTS
#define RTK_SHELL_API _declspec(dllimport)
#else
#define RTK_SHELL_API _declspec(dllexport)
#endif
#else
#define RTK_SHELL_API
#endif

typedef void * HRTK_SHELL;

#ifdef __cplusplus

class RTK_SHELL_API variable{
public:
	HRTK_SHELL *s;
	char name[256];
	enum v_type{
		vt_int,
		vt_float,
		vt_char_,	//char *
		vt_object,
	}type;
	union{
		int i_val;
		double f_val;
		char c_val[256];
		void * o_val;
	};
	variable(){
		type=vt_char_;
		*c_val=0;
		strcpy(name,"unknown_variable");
	}
	variable(int v, const char * n=0){
		type=vt_int;
		i_val=v;
		if(n) {
			strncpy(name,n,sizeof(name));
			name[sizeof(name) - 1] = 0;
		}
	};
	variable(const char *v, const char *n=0){
		type=vt_char_;
		strncpy(c_val,v,sizeof(c_val));
		c_val[sizeof(c_val) - 1] = 0;
		if(n) {
			strncpy(name,n,sizeof(name));
			name[sizeof(name) - 1] = 0;
		}
	}
	variable(double v, const char *n=0){
		type=vt_float;
		f_val=v;
		if(n) {
			strncpy(name,n,sizeof(name));
			name[sizeof(name) - 1] = 0;
		}
	}
	variable(void *v, const char *n=0){
		type=vt_object;
		o_val=v;
		if(n) {
			strncpy(name,n,sizeof(name));
			name[sizeof(name) - 1] = 0;
		}
	};
	virtual ~variable(){
	};
	void show();
	int set(const char * value);
	void get(char * buffer, int length);
};

RTK_SHELL_API void RTK_API shell_add_v(HRTK_SHELL, variable * v);
RTK_SHELL_API void RTK_API shell_del_v(HRTK_SHELL, variable * v);
RTK_SHELL_API int RTK_API shell_set_v(HRTK_SHELL, 
	const char * name,
	const char * value,
	variable ::v_type vt = variable::vt_char_
	);

RTK_SHELL_API variable * RTK_API shell_get_v(HRTK_SHELL, const char * name);
RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL, const char * name,int * value);
RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL, const char * name,char ** value);
RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL, const char * name,float * value);
RTK_SHELL_API int RTK_API shell_get_v(HRTK_SHELL, const char * name,void ** value);
#endif

struct shell_command{
	char * name;
	char * brief_description;
	int (*shell_proc)(char * arglist,void * pv);
};

#ifdef __cplusplus
	extern "C" {
#endif

RTK_SHELL_API HRTK_SHELL RTK_API shell_create(
	const shell_command * valid_cmds
	);

RTK_SHELL_API int RTK_API shell_execute(
	HRTK_SHELL, 
	const char * lbuffer,
	void * pv
	);

RTK_SHELL_API __bool RTK_API shell_execute_ex(
	HRTK_SHELL, 
	const char * lbuffer,
	void * pv,
	int * code
	);

RTK_SHELL_API void RTK_API shell_destroy(HRTK_SHELL);

#ifdef __cplusplus
}
#endif

#endif
