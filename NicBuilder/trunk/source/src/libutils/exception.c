/********************************************************************
	created:	2002/07/19
	created:	19:7:2002   11:38
	filename: 	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS\exception.c
	file path:	D:\DFVS2\SRC\VC\PUBLIC\SRC\LIBUTILS
	file base:	exception
	file ext:	c
	author:		Jackie Hunter, jhunter@tsinghua.org.cn
	
	purpose:	exception record dumping
*********************************************************************/
#include "precomp.h"

LIBUTILS_API int UTILS_API dump_context(PCONTEXT context)
{
	int		i;
	DWORD	flg;

	// utils_errorA("Flags = 0x%08x\n", context->ContextFlags);
	
	utils_errorA("------------------ Debug registers ------------------\n");
	if(CONTEXT_DEBUG_REGISTERS & context->ContextFlags){
		utils_errorA("Dr0=%08x\tDr1=%08x\n", context->Dr0, context->Dr1);
		utils_errorA("Dr2=%08x\tDr3=%08x\n", context->Dr2, context->Dr3);
		// utils_errorA("Dr4=%08x\tDr5=%08x\n", context->Dr4, context->Dr5);
		utils_errorA("Dr6=%08x\tDr7=%08x\n", context->Dr6, context->Dr7);
	}else{
		utils_errorA("N.A.\n");
	}

	utils_errorA("----------------- Segment registers -----------------\n");
	if(CONTEXT_SEGMENTS & context->ContextFlags){
		utils_errorA("ds=%04x\tes=%04x\tfs=%04x\tgs=%04x\n",
			context->SegDs, context->SegEs, context->SegFs, context->SegGs
		);
	}else{
		utils_errorA("N.A.\n");
	}

	utils_errorA("----------------- Integers registers ----------------\n");
	if(CONTEXT_INTEGER & context->ContextFlags){
		utils_errorA("eax=%08x\tebx=%08x\tecx=%08x\tedx=%08x\n", 
			context->Eax,
			context->Ebx,
			context->Ecx,
			context->Edx
		);
		utils_errorA("esi=%08x\tedi=%08x\n",
			context->Esi,
			context->Edi
		);
	}else{
		utils_errorA("N.A.\n");
	}

	utils_errorA("----------------- Control registers -----------------\n");
	if(CONTEXT_CONTROL & context->ContextFlags){
		utils_errorA("cs:eip=%04x:%08x\t", context->SegCs, context->Eip);
		utils_errorA("ss:esp=%04x:%08x\t", context->SegSs, context->Esp);
		utils_errorA("ebp=%08x\n", context->Ebp);
		utils_errorA("flg=");
		
		for(i=0, flg=context->EFlags; i<sizeof(context->EFlags)*8; i++, flg <<= 1){
			utils_errorA(flg & 0x80000000? "1" : "0");
		}
		utils_errorA("\n");
	}else{
		utils_errorA("N.A.\n");
	}
	return 0;
}

LIBUTILS_API int UTILS_API dump_exception(LPEXCEPTION_POINTERS info)
{
	DWORD	i;
	utils_errorA("Flags=0x%08x\tAddr=0x%08x\t", info->ExceptionRecord->ExceptionFlags, info->ExceptionRecord->ExceptionAddress);
	utils_errorA("Associated_Exception=%08x\n", info->ExceptionRecord->ExceptionRecord);
	utils_errorA("%d parameters:\n", info->ExceptionRecord->NumberParameters);
	for(i=0; i<info->ExceptionRecord->NumberParameters; i++){
		utils_errorA("Parameters[%02d] = 0x%08x\n", i, info->ExceptionRecord->ExceptionInformation[i]);
	}
	
	utils_errorA("\nDump of context \n\n");
	return dump_context(info->ContextRecord);
};