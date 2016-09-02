

#include "precomp.h"
#include "SerialComm.h"

#define BUFSIZE			128
#define MAX_ERROR	16
#define SIO_OK	0


CSerialPort::CSerialPort(int pno,char *setting)
{
	m_hFile = INVALID_HANDLE_VALUE;
	m_iPortNo = pno;
	m_OverlappedEvent = 0;
	m_iErrCount = 0;
	m_iSent = m_iSentBytes = m_iGot = m_iGotBytes = 0;
	memset(m_Setting,0,sizeof(m_Setting));
	if(setting == 0){
		strncpy(m_Setting, SPAC_DEFAULT_SETTING, sizeof(m_Setting));
		m_Setting[sizeof(m_Setting) - 1] = 0;
	}
	else{
		strncpy(m_Setting, setting, sizeof(m_Setting));
		m_Setting[sizeof(m_Setting) - 1] = 0;
	}
}

CSerialPort::~CSerialPort()
{
	close();
}

static char Parity(DCB * dcb)
{
	switch(dcb->Parity){
	case ODDPARITY:
		return 'o';
	case EVENPARITY:
		return 'e';
	case MARKPARITY:
		return 'm';
	case SPACEPARITY:
		return 'b';
	default:
		// case NONEPARITY:
		return 'n';
	}
}

static int Stopbits(int b)
{
	switch(b){
	case ONESTOPBIT:
		return 1;
	case ONE5STOPBITS:
		return 15;
	case TWOSTOPBITS:
		return 2;
	}
	return 0;
}

__bool CSerialPort::setup_dcb(const char *setting, DCB *dcb)
{
	DCB setDCB;
	
	memset(&setDCB, 0, sizeof(DCB));
	setDCB.DCBlength = sizeof(DCB);
	if(!BuildCommDCB(setting, &setDCB)){
		return __false;
	}

	*dcb = setDCB;
	
	dcb->DCBlength = sizeof(DCB);
	dcb->fBinary = TRUE;
	// dcb->fParity = TRUE;
	dcb->fOutxCtsFlow = FALSE;
	dcb->fOutxDsrFlow = FALSE;
	dcb->fDtrControl = DTR_CONTROL_ENABLE;
	dcb->fDsrSensitivity = FALSE;
	dcb->fTXContinueOnXoff = TRUE;
	dcb->fOutX = FALSE;
	dcb->fInX = FALSE;
	dcb->fErrorChar = FALSE;
	dcb->fNull = FALSE;
	dcb->fRtsControl = RTS_CONTROL_ENABLE;
	dcb->fAbortOnError = FALSE;
	dcb->fDummy2 = 0;
	dcb->wReserved = 0;
	dcb->XonLim = 0;
	dcb->XoffLim = 0;

	// dcb->ByteSize = setDCB.ByteSize;
	// dcb->Parity = setDCB.Parity;
	// dcb->StopBits = setDCB.StopBits;
	// dcb->XonChar = ;
	// dcb->XoffChar = 
	// dcb->ErrorChar = 
	// dcb->EofChar = 
	// dcb->EvtChar = 
	// dcb->wReserved1 = 

	// utils_trace("%08x\n", *((DWORD*)dcb + 2));
	return __true;
}

__bool CSerialPort::open()
{
	char			buf[64];
	DCB				dcb;
	COMMTIMEOUTS	tout;

	if(INVALID_HANDLE_VALUE != m_hFile){
		return __true;
	}
	sprintf(buf, "\\\\.\\COM%d", m_iPortNo);
	m_hFile = CreateFile(
		buf, 
		GENERIC_READ | GENERIC_WRITE,
		0,
		0,
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		0
		);
	utils_trace(
		"COM %d opened as 0x%08x\n", 
		m_iPortNo,
		m_hFile
		);
	if(INVALID_HANDLE_VALUE==m_hFile){
		return __false;
	}
	
	
	if( !setup_dcb(m_Setting, &dcb) ){
		close();
		utils_trace(
			"COM%d, error in setting string : %s, code %d\n", 
			m_iPortNo,
			m_Setting,
			GetLastError()
			);
		return __false;
	}

	if( !SetCommState(m_hFile, &dcb) ){
		utils_error(
			"COM%d, SetCommState failed with %d.\n",
			m_iPortNo,
			GetLastError()
			);
		close();
	}

	utils_trace(
		"Ok, COM%d setting applied : '%s'\n", 
		m_iPortNo,
		m_Setting
		);

	if( !GetCommTimeouts(m_hFile, &tout) ){
		close();
		utils_trace(
			"COM%d, Error in GetCommTimeouts, Code %d.\n", 
			m_iPortNo,
			GetLastError()
			);
		return __false;
	}

	tout.ReadIntervalTimeout = MAXDWORD;
	tout.ReadTotalTimeoutMultiplier = 0;
	tout.ReadTotalTimeoutConstant = 0;
	tout.WriteTotalTimeoutMultiplier = 10;
	tout.WriteTotalTimeoutConstant = 0;
	if(!SetCommTimeouts(m_hFile, &tout)){
		utils_trace(
			"COM%d, Error in SetCommTimeouts, Code %d.\n", 
			m_iPortNo,
			GetLastError()
			);
		close();
		return __false;
	}

	m_OverlappedEvent = new CEvent(0, TRUE, FALSE);
	if(!m_OverlappedEvent || !m_OverlappedEvent->Handle()){
		utils_trace(
			"COM%d, error %d creating overlapped event.\n",
			m_iPortNo,
			GetLastError()
			);
		close();
		return __false;
	}

	if( !PurgeComm(
		m_hFile, 
		PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
		)
	){
		utils_error(
			"COM%d, Warning : PurgeComm failed with %d\n",
			m_iPortNo,
			GetLastError()
			);
	}

	COMMCONFIG	cfg;
	DWORD size;
	cfg.dwSize = size = sizeof(cfg);
	if( !GetCommConfig(m_hFile, &cfg, &size) ){
		utils_error(
			"Warning : GetCommConfig failed with %d\n", 
			GetLastError()
			);
	}else{
		utils_trace(
			"COM%d setting is : '%d,%c,%d,%d(%08x)'\n",
			m_iPortNo,
			cfg.dcb.BaudRate,
			Parity(&cfg.dcb),
			cfg.dcb.ByteSize,
			Stopbits(cfg.dcb.StopBits),
			*((DWORD*)&cfg.dcb + 2)
			);
	}

	return __true;
}

__bool CSerialPort::close()
{
	if(INVALID_HANDLE_VALUE != m_hFile){
		flush();
		CloseHandle(m_hFile);
		m_hFile = INVALID_HANDLE_VALUE;
	}
	if(m_OverlappedEvent){
		delete m_OverlappedEvent;
		m_OverlappedEvent = 0;
	}

	return __true;
}

__bool CSerialPort::flush()
{
	if(INVALID_HANDLE_VALUE != m_hFile){
		PurgeComm(
			m_hFile, 
			PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR
			);
	}

	utils_trace("COM%d flushed.\n", m_iPortNo);
	
	return __true;
}



__bool CSerialPort::write_timeout(DWORD TotalTimeouts)
{
	COMMTIMEOUTS tout;
	if(INVALID_HANDLE_VALUE == m_hFile){
		SetLastError(ERROR_INVALID_HANDLE);
		return __false;
	}
	if(!GetCommTimeouts(m_hFile, &tout)){
		return __false;
	}
	tout.WriteTotalTimeoutConstant = TotalTimeouts;
	if( !SetCommTimeouts(m_hFile, &tout)){
		return __false;
	}
	return __true;
}

void CSerialPort::check_error()
{
	DWORD len;
	COMSTAT stat;
	char errStr[256];

	if(INVALID_HANDLE_VALUE == m_hFile){
		SetLastError(ERROR_INVALID_HANDLE);
		return;
	}
	memset(&stat, 0, sizeof(stat));
	if( !ClearCommError(m_hFile, (LPDWORD)&len, &stat) ){
		utils_trace(
			"Err in ClearCommError, code %d\n", 
			GetLastError()
			);
		return;
	}
	ClearCommBreak(m_hFile);
	
	if(!len){
		return;
	}

	errStr[0] = 0;

#define chk(e) \
	do{if(len & e) {\
		strcat(errStr, #e); \
		strcat(errStr, " ");\
	}}while(0)
	
	chk(CE_BREAK);
	chk(CE_DNS);
	chk(CE_FRAME);			
	chk(CE_IOE);
	chk(CE_MODE);
	chk(CE_OOP);
	chk(CE_OVERRUN);
	chk(CE_PTO);
	chk(CE_RXOVER);
	chk(CE_RXPARITY);
	chk(CE_TXFULL);

	utils_trace(
		"Err 0x%08x(%s) on COM%d, COMSTAT=(0x%08x, 0x%08x, 0x%08x)\n",
		len,
		errStr,
		m_iPortNo,
		*((DWORD*)&stat),
		stat.cbInQue,
		stat.cbOutQue
		);

	if(len & CE_RXOVER){
		PurgeComm(m_hFile, PURGE_RXCLEAR | PURGE_RXABORT);
	}

	if(len & CE_TXFULL){
		PurgeComm(m_hFile, PURGE_TXCLEAR | PURGE_TXABORT);
	}
}

__bool CSerialPort::read(
	char * buf, 
	__uint len,
	__uint * bytes
	)
{
	OVERLAPPED ovl;
	if(INVALID_HANDLE_VALUE == m_hFile){
		SetLastError(ERROR_INVALID_HANDLE);
		return __false;
	}
	memset(&ovl, 0, sizeof(ovl));
	ovl.hEvent = m_OverlappedEvent->Handle();
	if(!ReadFile(m_hFile, buf, len, (LPDWORD)bytes, &ovl)){
		if(ERROR_IO_PENDING == GetLastError()){
			if( !GetOverlappedResult(
				m_hFile,
				&ovl,
				(LPDWORD)bytes,
				TRUE
				))
			{
				*bytes = 0;
				return __false;
			}
		}

		check_error();
	}
	m_iGot++;
	m_iGotBytes += *bytes;

	utils_debug(
		">> COM%d, %d bytes reading request...%d read.",
		m_iPortNo,
		len,
		*bytes
		);

	utils_debug("\n");

	return __true;
}

/*
	overlapped write
*/
__bool CSerialPort::write(
	const char * buf, 
	__uint len, 
	__uint * bytes
	)
{
	OVERLAPPED	ovl;

	if(INVALID_HANDLE_VALUE == m_hFile){
		SetLastError(ERROR_INVALID_HANDLE);
		return __false;
	}
	memset(&ovl, 0, sizeof(ovl));
	ovl.hEvent = m_OverlappedEvent->Handle();
	if(!WriteFile(m_hFile, buf, len, (LPDWORD)bytes, &ovl)){
		if(ERROR_IO_PENDING == GetLastError()){
			if(!GetOverlappedResult(
				m_hFile,
				&ovl,
				(LPDWORD)bytes,
				TRUE
				))
			{
				*bytes = 0;
				return 0;
			}
		}

		check_error();
	}
	
	m_iSent++;
	m_iSentBytes += *bytes;

	utils_debug(
		">> COM%d, %d bytes writing request...%d written.\n",
		m_iPortNo,
		len,
		*bytes,
		buf
		);


	
	return __true;
}