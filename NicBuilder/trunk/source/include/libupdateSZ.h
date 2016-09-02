// 下列 ifdef 块是创建使从 DLL 导出更简单的
// 宏的标准方法。此 DLL 中的所有文件都是用命令行上定义的 LIBUPDATESZ_EXPORTS
// 符号编译的。在使用此 DLL 的
// 任何其他项目上不应定义此符号。这样，源文件中包含此文件的任何其他项目都会将
// LIBUPDATESZ_API 函数视为是从 DLL 导入的，而此 DLL 则将用此宏定义的
// 符号视为是被导出的。

#ifndef _libupdateSZ_h_
#define _libupdateSZ_h_


#ifdef LIBUPDATESZ_EXPORTS
#define LIBUPDATESZ_API __declspec(dllexport)
#else
#define LIBUPDATESZ_API __declspec(dllimport)
#endif

// 此类是从 libupdateSZ.dll 导出的
class LIBUPDATESZ_API ClibupdateSZ {
public:
	ClibupdateSZ(void);
	// TODO: 在此添加您的方法。
};

extern LIBUPDATESZ_API int nlibupdateSZ;

LIBUPDATESZ_API int fnlibupdateSZ(void);

//=== Ping功能相关

// 
class LIBUPDATESZ_API CPingSZ {
public:
	CPingSZ(void);
	~CPingSZ(void);
	// TODO: 在此添加您的方法。

	// 输入为IP地址、超时时间
	// 超时时间：在超时时间内，若没有ping通，则会一直不停地ping
	int pingSZ(const char* IP, long timeout);
	int delay_pingSZ(const char*IP, long timeout);

	ACE_Ping_Socket* ping_ptr;
	ACE_INET_Addr * addr_ptr;

	int m_iter_count;    // 没ping上的时间计数 
};


#endif