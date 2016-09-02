#ifndef __pmc_license_h__
#define __pmc_license_h__

#include <librtk.h>
#include "pmckey.h"

#define SIGNATURE 0xdef3c4a0

#include <pshpack1.h>
/* a 128-byte record */
struct pmc_license_t{
	__i32 sig;
	__i32 count;
	__i32 runLevel;
	__i32 devLevel;
	char comment[64];
	__u8 reserved[176];
};
#include <poppack.h>

BOOL genLicense(const char * file, struct pmc_license_t * lic);
typedef __bool (__cdecl *ENUM_LIC_PROC)(const char * file, int entryId, struct pmc_license_t * lic, int context);
LIBPMCKEY_API int enumLicense(const char * file, ENUM_LIC_PROC p, int con);
int enumLocalLicense(const char * file, ENUM_LIC_PROC p, int con);
LIBPMCKEY_API BOOL insertLicense(const char * file, struct pmc_license_t * lic);
LIBPMCKEY_API BOOL removeLicense(const char * file, int id, int count);
LIBPMCKEY_API void createNullLicenseFile(const char * file);

#endif

