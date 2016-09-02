#ifndef __pmckey_h_
#define __pmckey_h_

#ifdef _WIN32
	#ifdef LIBPMCKEY_EXPORTS
		#define LIBPMCKEY_API      __declspec(dllexport)
	#else
		#define LIBPMCKEY_API      __declspec(dllimport)		
	#endif
	#define PMCKEY_API _stdcall
#else
	#define LIBPMCKEY_API       
	#define PMCKEY_API
#endif

#ifdef __cplusplus
extern "C"{
#endif
	LIBPMCKEY_API unsigned char PMCKEY_API init_key();
	LIBPMCKEY_API void PMCKEY_API get_key(unsigned char *run, int *runLevel, unsigned char *dev, int *devLevel);
	LIBPMCKEY_API void PMCKEY_API set_product_type();
	LIBPMCKEY_API int PMCKEY_API get_product_type();
#ifdef __cplusplus
}
#endif

#endif
