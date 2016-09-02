//MBX 头文件
#include "MbxApi.h"
#include "MbxCom.h"
#include "MbxError.h"
#include "MbxGnPkt.h"
#include "MbxIoErr.h"
#include "Netbios.h"
#include "Netlib.h"

#ifdef __cplusplus
extern "C"{
#endif

//输出函数

//-------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxOpenDeviceDescriptionEnum(                        
	PHANDLE     phEnum				// Device description enumerator handle
    );

//-------------------------------------------------------------------------------                          
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxCloseDeviceDescriptionEnum(                       
    HANDLE		hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

//-------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxResetDeviceDescriptionEnum(
    HANDLE  hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

//-------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxDeviceDescriptionEnumNext(
    HANDLE		hEnum               // Handle from MbxOpenDeviceDescriptionEnum()
    );

//------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxReadDeviceDescriptionA(
    HANDLE       hEnum,				// Handle from MbxOpenDeviceDescriptionEnum()
    PBOOL		pbPhysDevice,		// Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    PDWORD   pdwDeviceNumber,		// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    LPSTR   pszNetworkProtocol,		// Ptr to network protocol description string
    PDWORD  pdwNetworkProtocolSize,	// Network protocol string size in bytes
    LPSTR   pszAdapterCard,			// Ptr to adapter card description string
    PDWORD  pdwAdapterCardSize,		// Adapter card string size in bytes
    LPSTR   pszDeviceName,			// Ptr to device name string
    PDWORD  pdwDeviceNameSize,		// Device name string size in bytes
    LPSTR   pszDeviceDesc,			// Ptr to device description string
    PDWORD  pdwDeviceDescSize		// Device description string size in bytes
    );

//---------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec (dllexport)
WINAPI
dllMbxReadDeviceDescriptionW(                        
    HANDLE       hEnum,				// Handle from MbxOpenDeviceDescriptionEnum()
    PBOOL    pbPhysDevice,			// Ptr to physical/local device flag:
                                    // IN:
                                    // TRUE  -  Request description of a physical device
                                    //          associated with the local device
                                    //          (e.g. remote physical device)
                                    // FALSE -  Request description of a local device; It
                                    //          could be physical or logical device.
                                    // OUT:
                                    // TRUE  -  Requested description is from the
                                    //          physical device.
                                    // FALSE -  Requested description is from the
                                    //          logical device.
    PDWORD   pdwDeviceNumber,		// Ptr to device number (0,1,2,...)
                                    // IN:
                                    // if device # = -1 then reads currently enumerated device
                                    // description.
                                    // OUT:
                                    // Physical/Local device number
    LPWSTR  pszNetworkProtocol,		// Ptr to network protocol description string
    PDWORD  pdwNetworkProtocolSize,	// Network protocol string size in bytes
    LPWSTR  pszAdapterCard,			// Ptr to adapter card description string
    PDWORD  pdwAdapterCardSize,		// Adapter card string size in bytes
    LPWSTR  pszDeviceName,			// Ptr to device name string
    PDWORD  pdwDeviceNameSize,		// Device name string size in bytes
    LPWSTR  pszDeviceDesc,			// Ptr to device description string
    PDWORD  pdwDeviceDescSize		// Device description string size in bytes
    );

//----------------------------------------------------------------------------------
/*#ifdef UNICODE
#define MbxReadDeviceDescription  MbxReadDeviceDescriptionW
#else
#define MbxReadDeviceDescription  MbxReadDeviceDescriptionA
#endif // !UNICODE
这个函数库中没有！！！

MBXDLLAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
dllMbxReadPhysicalDeviceDescriptionA(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPSTR       pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPSTR       pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPSTR       pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPSTR       pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    )
{
	return MbxReadPhysicalDeviceDescriptionA(
    pdwDeviceNumber,
    pszNetProtocol,
    pdwNetProtocolSize,
    pszAdapterCard,
    pdwAdapterCardSize,
    pszDeviceName,
    pdwDeviceNameSize,
    pszDeviceDesc,
    pdwDeviceDescSize
	);
};


MBXDLLAPI
DWORD                               // MBX_SUCCESS if successful, else error code
WINAPI
dllMbxReadPhysicalDeviceDescriptionW(
    IN OUT PDWORD   pdwDeviceNumber,// Ptr to device number (0,1,2,...)
                                    // IN:  Logical device number
                                    // OUT: Physical device number
    OUT LPWSTR      pszNetProtocol, // Ptr to network protocol description string
    IN OUT  PDWORD  pdwNetProtocolSize,// Network protocol string size in bytes
    OUT LPWSTR      pszAdapterCard, // Ptr to adapter card description string
    IN OUT  PDWORD  pdwAdapterCardSize,// Adapter card string size in bytes
    OUT LPWSTR      pszDeviceName,  // Ptr to device name string
    IN OUT  PDWORD  pdwDeviceNameSize,// Device name string size in bytes
    OUT LPWSTR      pszDeviceDesc,  // Ptr to device description string
    IN OUT  PDWORD  pdwDeviceDescSize// Device description string size in bytes
    )
{
	return MbxReadPhysicalDeviceDescriptionW(
    pdwDeviceNumber,
    pszNetProtocol,
    pdwNetProtocolSize,
    pszAdapterCard,
    pdwAdapterCardSize,
    pszDeviceName,
    pdwDeviceNameSize,
    pszDeviceDesc,
    pdwDeviceDescSize
    );
};
*/

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxOpen(
    DWORD       dwDeviceNumber,		// Device (adapter card) number (0,1,2,...)
    PHANDLE     phDevice			// Ptr to device driver handle
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxClose(
    HANDLE  hDevice					// Device driver handle from MbxOpen()
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxOpenMp(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    PMBXPATHID	pPathID				// Pointer to DM/PM path ID
    );

//-----------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxCloseMp(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID				// DM/PM path ID from MbxOpenMp()
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWriteMpCmdWait(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,			// DM/PM path ID from MbxOpenMp()
    USHORT      Routing[],			// Ptr to array of 5 (16-bit) routing codes
    DWORD       dwRoutingSize,		// Routing array size in bytes
    PMBXPKT     pCmdPkt,			// Ptr to command packet
    DWORD       dwCmdPktSize,		// Command packet size in bytes
    PMBXPKT     pReplyPkt,			// Ptr to reply packet
    PDWORD		pdwReplyPktSize,	// Ptr to reply packet size in bytes
    DWORD       dwTimeout			// Time-out in milliseconds
    );

//-----------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWriteMpCmd(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,			// DM/PM path ID from MbxOpenMp()
    USHORT      Routing[],			// Ptr to array of 5 (16-bit) routing codes
    DWORD       dwRoutingSize,		// Routing array size in bytes
    PMBXPKT     pCmdPkt,			// Ptr to command packet
    DWORD       dwCmdPktSize,		// Command packet size in bytes
    PMBXPKT     pReplyPkt,			// Ptr to reply packet
    DWORD       dwReplyPktSize,		// Reply packet size in bytes
    DWORD       dwTimeout,			// Time-out in milliseconds
    LPOVERLAPPED lpOverlapped		// Points to an OVERLAPPED structure
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWriteMpCmdCB(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,			// DM/PM path ID from MbxOpenMp()
    USHORT      Routing[],			// Ptr to array of 5 (16-bit) routing codes
    DWORD       dwRoutingSize,		// Routing array size in bytes
    PMBXPKT     pCmdPkt,			// Ptr to command packet
    DWORD       dwCmdPktSize,		// Command packet size in bytes
    DWORD       dwTimeout,			// Time-out in milliseconds
    DWORD       dwParameter,		// Parameter to be passed to the CallBack function
    PFMBXCALLBACK   pCallBack		// Ptr to completion callback routine
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadMpReply(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,			// DM/PM path ID from MbxOpenMp()
    PMBXPKT     pReplyPkt,			// Ptr to reply packet
    PDWORD   pdwReplyPktSize		// Ptr to reply packet size in bytes
    );

//-----------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxAbortMp(
	HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID				// DM/PM path ID from MbxOpenMp()
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxOpenSp(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID				// DS/PS path ID
    );

//-----------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxCloseSp(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID				// DS/PS path ID as used in MbxOpenSp()
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWaitForSpCmd(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    PDWORD      pdwCmdPktSize,		// Ptr to command packet size in bytes
    DWORD       dwTimeout,			// Time-out in milliseconds
    LPOVERLAPPED lpOverlapped		// Points to an OVERLAPPED structure
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWaitForSpCmdCB(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID   *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    DWORD       dwTimeout,			// Time-out in milliseconds
    DWORD       dwParameter,		// Parameter to be passed to the CallBack function
    PFMBXCALLBACK   pCallBack		// Ptr to completion callback routine
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxAbortSp(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID				// DS/PS path ID as used in MbxOpenSp()
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadSpCmdWait(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    USHORT      Routing[],			// Ptr to array of 5 (16-bit) routing codes + msg source
    PDWORD		pdwRoutingSize,		// Ptr to routing array size in bytes
    PMBXPKT     pCmdPkt,			// Ptr to command packet
    PDWORD		pdwCmdPktSize,		// Ptr to command packet size in bytes
    DWORD       dwTimeout			// Time-out in milliseconds
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadSpCmd(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    PMBXSPPKT   pCmdSpPkt,			// Ptr to combined routing array and command packet
    DWORD       dwCmdSpPktSize,		// Command packet size in bytes
    DWORD       dwTimeout,			// Time-out in milliseconds
    LPOVERLAPPED lpOverlapped		// Points to an OVERLAPPED structure
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWriteSpReply(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    PMBXPKT     pReplyPkt,			// Ptr to reply packet
    DWORD       dwReplyPktSize		// Reply packet size in bytes
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxSetSpLogin(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    MBXPATHID  *pPathID,				// DS/PS path ID as used in MbxOpenSp()
    UCHAR       byStatus			// 1 = set login, 0 = clear login
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxWriteGlobalData(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    BOOL        bSetMaxSize,		// If TRUE, set the max size of global data array
    DWORD       dwStartOffset,		// Start byte offset into global data buffer
    PUCHAR      pGlbBuf,			// Pointer to global data buffer
    DWORD       dwByteCount			// Number of global data bytes to write
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadGlobalDataWait(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    USHORT      wNode,				// Node number
    DWORD       dwStartOffset,		// Start byte offset into global data buffer
    PUCHAR      pGlbBuf,			// Pointer to global data buffer
    PDWORD		pdwByteCount,		// Pointer to number of global data bytes
    DWORD       dwTimeout			// Time-out in milliseconds
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadGlobalData(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    USHORT      wNode,				// Node number
    DWORD       dwStartOffset,		// Start byte offset into global data buffer
    PUCHAR      pGlbBuf,			// Pointer to global data buffer
    DWORD       dwByteCount,		// Number of global data bytes to be read
    DWORD       dwTimeout,			// Time-out in milliseconds
    LPOVERLAPPED lpOverlapped		// Points to an OVERLAPPED structure
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxAbortReadGlobalData(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    USHORT      wNode				// Node number
    );

//-------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxGetOverlappedResult(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    LPOVERLAPPED lpOverlapped,		// Address of overlapped structure
    PDWORD		pdwBytesTransferred,// Address of actual bytes count
    BOOL        bWait				// Wait flag
    );

//-------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxDevStatistics(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    PUCHAR      pStsBuf,			// Pointer to network statistics buffer
    PDWORD		pdwStsBufSize		// Pointer to length of buffer in bytes
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadDeviceConfig(
	HANDLE      hDevice,			// Device driver handle from MbxOpen()
    PMBXDEVCFG  pDevCfg,			// Ptr to device driver configuration structure
    PDWORD		pdwDevCfgSize		// Ptr to driver configuration structure size in bytes
    );

//------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxReadDeviceDiagnostics(
    HANDLE      hDevice,			// Device driver handle from MbxOpen()
    PMBXDEVDIAG pDevDiag,			// Ptr to driver diagnostics structure
    PDWORD		pdwDevDiagSize		// Ptr to driver diag counters structure size in bytes
    );

//-------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxGetErrorMessageA(
    DWORD       dwIoStatus,			// I/O status code
    DWORD       dwLanguageId,		// language identifier for requested message 
    LPSTR       pszMsgBuf,			// Pointer to message buffer 
    PDWORD		pdwMsgBufSize		// Size of message buffer 
    );

//-------------------------------------------------------------------------------------
DWORD                               // MBX_SUCCESS if successful, else error code
_declspec(dllexport)
WINAPI
dllMbxGetErrorMessageW(
    DWORD       dwIoStatus,			// I/O status code
    DWORD       dwLanguageId,		// language identifier for requested message 
    LPWSTR      pszMsgBuf,			// Pointer to message buffer 
    PDWORD		pdwMsgBufSize		// Size of message buffer 
    );
//-------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif