/*
lava language system dll interface
panxh, Feb 2006
*/

#ifndef __lava_script_h__
#define __lava_script_h__

#ifdef _WIN32
#ifdef LAVASCR_EXPORTS
#define LAVASCR_API __declspec(dllexport)
#else
#define LAVASCR_API __declspec(dllimport)
#endif
#else
#define LAVASCR_API 
#endif

typedef struct __LAVA_MODULE * HLAVA_MODULE;

LAVASCR_API HLAVA_MODULE lavaCompileFile(const char * cmdLine);
LAVASCR_API HLAVA_MODULE lavaCompileText(const char * text);
LAVASCR_API int lavaSaveModule(const char * fileName, HLAVA_MODULE mod);
LAVASCR_API HLAVA_MODULE lavaLoadModule(const char * fileName);
LAVASCR_API HLAVA_MODULE lavaLoadModuleFromStream(const __u8 * str, __uint len);
LAVASCR_API void lavaFreeModule(HLAVA_MODULE h);

#endif

