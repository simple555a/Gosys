// ���� ifdef ���Ǵ���ʹ�� DLL �������򵥵�
// ��ı�׼�������� DLL �е������ļ��������������϶���� LIBUPDATESZ_EXPORTS
// ���ű���ġ���ʹ�ô� DLL ��
// �κ�������Ŀ�ϲ�Ӧ����˷��š�������Դ�ļ��а������ļ����κ�������Ŀ���Ὣ
// LIBUPDATESZ_API ������Ϊ�Ǵ� DLL ����ģ����� DLL ���ô˺궨���
// ������Ϊ�Ǳ������ġ�

#ifndef _libupdateSZ_h_
#define _libupdateSZ_h_


#ifdef LIBUPDATESZ_EXPORTS
#define LIBUPDATESZ_API __declspec(dllexport)
#else
#define LIBUPDATESZ_API __declspec(dllimport)
#endif

// �����Ǵ� libupdateSZ.dll ������
class LIBUPDATESZ_API ClibupdateSZ {
public:
	ClibupdateSZ(void);
	// TODO: �ڴ�������ķ�����
};

extern LIBUPDATESZ_API int nlibupdateSZ;

LIBUPDATESZ_API int fnlibupdateSZ(void);

//=== Ping�������

// 
class LIBUPDATESZ_API CPingSZ {
public:
	CPingSZ(void);
	~CPingSZ(void);
	// TODO: �ڴ�������ķ�����

	// ����ΪIP��ַ����ʱʱ��
	// ��ʱʱ�䣺�ڳ�ʱʱ���ڣ���û��pingͨ�����һֱ��ͣ��ping
	int pingSZ(const char* IP, long timeout);
	int delay_pingSZ(const char*IP, long timeout);

	ACE_Ping_Socket* ping_ptr;
	ACE_INET_Addr * addr_ptr;

	int m_iter_count;    // ûping�ϵ�ʱ����� 
};


#endif