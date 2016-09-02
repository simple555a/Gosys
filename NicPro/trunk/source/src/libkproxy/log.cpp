/*
	Log Manager
	by J.Hunter, 2003/9/20
*/
#include "precomp.h"

static void _show_log(struct klog_t * l, int entries)
{
	int i;
	struct klog_ent_t * e;

	i = l->tail - 1;
	while(i != l->tail){
		if(i < 0){
			i = entries - 1;
		}
		e = &l->logs[i];
		if(e->tm != 0){
#ifdef _WIN32
			{
				SYSTEMTIME st;
				FileTimeToSystemTime(
					(PFILETIME)&e->tm,
					&st
					);
				utils_trace("%04d/%02d/%02d %02d:%02d:%02d %03d",
					st.wYear,
					st.wMonth,
					st.wDay,
					st.wHour,
					st.wMinute,
					st.wSecond,
					st.wMilliseconds
					);
			}
#else
			utils_trace("%08x %08x",
				(__uint)e->tm >> 32,
				(__uint)e->tm & 0xffffffff
				);
#endif
			utils_trace(" - ");
			utils_trace("%08x %08x %08x %08x,(%s)", 
				e->c[0], 
				e->c[1],
				e->c[2],
				e->c[3],
				_errmsg(e->c[0])
				);
			if(event_module(e->c[0]) == MODNUM_KERNEL){
				switch(event_code(e->c[0])){
					case F8_MODE_SWITCH:
						//utils_trace("%s->%s",ke_get_mode_name(e->c[1]),ke_get_mode_name(e->c[2]));
						break;
				}
			}
			utils_trace("\n");
		}

		i--;
	}
}

int on_log(char *v, void *k)
{
	CArgsEx a;
	struct klog_t l;

	_shproc_prolog_();

	a.parse(v, FS);
	if(a.argc < 2){
		utils_error("Usage : log <show> | <clear>\n");
		return F8_SYNTAX;
	}
	
	if(!strcmpi(a[1], "clear")){
		cmd.cmd = F8_CLEAR_LOG_BUF;
		cmd.isize = 0;
		__vcall__(
			proxy_adapter, 
			exec, 
			(proxy_adapter, &cmd, psp->timeout)
			);
	}else{
		cmd.cmd = F8_GET_LOG_BUF;
		cmd.isize = 0;
		cmd.obuffer = (char*)&l;
		cmd.osize = sizeof(struct klog_t);
		__vcall__(
			proxy_adapter, 
			exec_remote, 
			(proxy_adapter, &cmd, psp->timeout)
			);

		if(successful){
			_show_log(&l, KERN_LOG_BUF_SIZE);
		}
	}
	
	return cmd.retcode;
}