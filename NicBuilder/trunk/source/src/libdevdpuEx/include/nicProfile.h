

#ifndef __NICSYS_PROFILE_H__
#define __NICSYS_PROFILE_H__

#ifndef UTILS_API
#define UTILS_API
#endif

CDECL_BEGIN

	extern UTILS_API int read_string_key(
		const char * lpAppName,        // points to section name
		const char * lpKeyName,        // points to key name
		const char * lpDefault,        // points to default string
		char * lpReturnedString, // points to destination buffer
		int nSize,               // size of destination buffer
		const char * lpFileName        // points to initialization filename
		);

	extern UTILS_API int write_string_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		const char * lpString,   // pointer to string to add
		const char * lpFileName  // pointer to initialization filename
		);

	extern UTILS_API int read_int_key(
		const char * lpAppName,        // points to section name
		const char * lpKeyName,        // points to key name
		int value,
		const char * lpFileName        // points to initialization filename
		);

	extern UTILS_API int write_int_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		int    value,
		const char * lpFileName        // points to initialization filename
		);

	extern UTILS_API int read_dbl_key(
		const char * lpAppName,        // points to section name
		const char * lpKeyName,        // points to key name
		double * value,
		const char * lpFileName        // points to initialization filename
		);

	extern UTILS_API int write_dbl_key(
		const char * lpAppName,  // pointer to section name
		const char * lpKeyName,  // pointer to key name
		double value,
		const char * lpFileName        // points to initialization filename
		);




CDECL_END
#endif
