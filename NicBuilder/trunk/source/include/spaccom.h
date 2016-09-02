/*
  PC based Monitoring System source file
  version 1.0
  by J.Hunter,jhunter@263.net,July 2001

  Declaration of the structures used in communication between SPAC clients
  and SPAC server.
*/
#ifndef __spaccom_h__
#define __spaccom_h__

#include <rtkdefs.h>
#include <libspac.h>
#include <pshpack4.h>

#define rtkm_spac_buffer_length 8192

/*
  client --> server
*/
struct spac_command{
	int  cmd_code;
	int  param[4];        
};

/*
  server --> client
*/
struct spac_response{
	int  ret_code;
	int  param[4];        
};

/*
  shared memory
*/
struct spac_shm{
	int  port_no;
	int  is_open;
	void * reserved[4];
	struct spac_command  i;
	struct spac_response o;
	char buffer[rtkm_spac_buffer_length];
};

/*
  max ports supported
*/
#define rtkm_max_spac_ports 16
LIBSPAC_API extern struct spac_shm * ports;

#include <poppack.h>

#endif
