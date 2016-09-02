#ifndef  __PMC_SERIALCOMM_H__
#define __PMC_SERIALCOMM_H__

#include"pmc.h"

class CSerialPort
{
public:
	CSerialPort(int pno,char *setting  =0);
	virtual ~CSerialPort();
	
	__bool open();
	__bool reset();
	__bool close();
	__bool opened()
	{
		return INVALID_HANDLE_VALUE == m_hFile? __false : __true;
	}
	__bool write_timeout(DWORD TotalTimeouts);

	void check_error();

	__bool read(
		char * buf, 
		__uint len,
		__uint * bytes
		);
	__bool write(
		const char * buf, 
		__uint len,
		__uint * bytes
		);
	__bool flush();

	__bool setup_dcb(const char *setting, DCB *dcb);

protected:
	char		m_Setting[128];
	int			m_iPortNo;
	CEvent		* m_OverlappedEvent;
	HANDLE		m_hFile;

	int			m_iErrCount;
	__bool		m_bOpened;
	int			m_iErrNo;
	int			m_iSent, m_iGot, m_iSentBytes, m_iGotBytes;
};

#endif