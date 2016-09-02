#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#define INCLUDE_TCP_STREAM
#endif

#define HAVE_STDIO

#ifdef HAVE_STDIO
#include <stdio.h>
#include <assert.h>
#endif

#include <string.h>
#include <c_bstr.h>
#include <treelist.h>
#include <streamable.h>

