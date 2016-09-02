#include "precomp.h"

#pragma data_seg("spac_shared")
LIBSPAC_API char _ports[rtkm_max_spac_ports*sizeof(spac_shm)]="";
#pragma data_seg()
#pragma comment(linker, "/section:spac_shared,rws")

LIBSPAC_API spac_shm * ports=(spac_shm * )_ports;
