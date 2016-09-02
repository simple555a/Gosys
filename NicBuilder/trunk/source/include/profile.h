/*
    profile support for Non-Win32 platforms
    the procs here also removes the 64kB barrier of INI file under Win9X
    Jackie, jhunter@tsinghua.org.cn
    Revision history :
    created on March 2001
    added class CProfile on April 16,2001
*/
#ifndef __profile_h__
#define __profile_h__

#ifndef LIBRTK_API
#define LIBRTK_API
#endif

#ifdef __cplusplus
extern "C"{
#endif

extern LIBRTK_API int read_string_key(
  const char * lpAppName,        // points to section name
  const char * lpKeyName,        // points to key name
  const char * lpDefault,        // points to default string
  char * lpReturnedString, // points to destination buffer
  int nSize,               // size of destination buffer
  const char * lpFileName        // points to initialization filename
);

extern LIBRTK_API int write_string_key(
  const char * lpAppName,  // pointer to section name
  const char * lpKeyName,  // pointer to key name
  const char * lpString,   // pointer to string to add
  const char * lpFileName  // pointer to initialization filename
);

extern LIBRTK_API int read_int_key(
  const char * lpAppName,        // points to section name
  const char * lpKeyName,        // points to key name
  int value,
  const char * lpFileName        // points to initialization filename
);

extern LIBRTK_API int write_int_key(
  const char * lpAppName,  // pointer to section name
  const char * lpKeyName,  // pointer to key name
  int    value,
  const char * lpFileName        // points to initialization filename
);

extern LIBRTK_API int read_dbl_key(
  const char * lpAppName,        // points to section name
  const char * lpKeyName,        // points to key name
  double * value,
  const char * lpFileName        // points to initialization filename
);

extern LIBRTK_API int write_dbl_key(
  const char * lpAppName,  // pointer to section name
  const char * lpKeyName,  // pointer to key name
  double value,
  const char * lpFileName        // points to initialization filename
);

#ifdef __cplusplus
}
#endif

#endif
