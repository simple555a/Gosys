/*
	PC based Monitoring System source file
	version 1.0
	by J.Hunter,jhunter@263.net,May 2001
*/
#include "precomp.h"

int on_restart(char *, void *_p)
{
	PCRTK_PACKET	p;
	static RTK_GUID last_id;
	
	/* suppress another restart */
	p = (PCRTK_PACKET)_p;
	if(last_id == p->guid){
		return -1;
	}
	last_id = p->guid;
	serv.Restart();
	return 0;
}

int on_rename(char * v, void *_p)
{
	PCRTK_PACKET	p;
	static RTK_GUID last_id;
	
	/* suppress another restart */
	p = (PCRTK_PACKET)_p;
	if(last_id == p->guid){
		return -1;
	}
	last_id = p->guid;
	CArgs args;
	parse_arg_ex(v, &args);
	if(args.argc != 2){
		return -1;
	}
	WritePrivateProfileString(
		"PMC",
		"ServerName",
		args.argv[1],
		get_config_file()
		);
	serv.Restart();
	return 0;
}

int on_pause(char *, void *_p)
{
	PCRTK_PACKET	p;
	static RTK_GUID last_id;
	
	p = (PCRTK_PACKET)_p;
	if(last_id == p->guid){
		return -1;
	}
	last_id = p->guid;
	// to be implemented
	return 0;
}

int on_resume(char *, void *_p)
{
	PCRTK_PACKET	p;
	static RTK_GUID last_id;
	
	p = (PCRTK_PACKET)_p;
	if(last_id == p->guid){
		return -1;
	}
	last_id = p->guid;
	// to be implemented
	return 0;
}

int on_exit(char *,void *)
{
	serv.Stop();
	return 0;
}
