// SearchDevice.h: interface for the CSearchDevice class.
//
//////////////////////////////////////////////////////////////////////

#ifndef _SEARCHDEVICE_H
#define _SEARCHDEVICE_H

#pragma once


class   CUDPSocket	;
class	CUDPSendSocket;
#define  WM_CLOSE_MSG   WM_USER+1503
#define  WM_RECV_DATA   WM_USER+1502
/*
typedef struct __str_packet__{
	uint32_t	sync;		//  //synchronize fields
	uint32_t	msg_type;	//data packet type field
	uint32_t	sub_type;	//data packet sub type field
	uint32_t	len;		//data domain valid len
	uint32_t	err_code;	//err code;

	uint32_t		net_type;		//0-static ip, 1-normal_dhcp, 2-pppoe_dhcp
	unsigned short  port;
	unsigned char	host_name[24];		//host name
	unsigned char	mac_addr[24];		//host mac addr
	unsigned char	ip_addr[16];		//ip address
	unsigned char	net_mask[16];		//server net mask
	unsigned char	gw_addr[16];		//gateway address
	
	//////////////////////////////
		unsigned short       webport;           //webport
	unsigned char       devtype[20];
	
	unsigned char	buf[200-102];//buf[200-124];
}_STR_PACKET_;
*/
class AFX_EXT_CLASS CSearchDevice  
{
public:
	          CSearchDevice();
	virtual  ~CSearchDevice();

	//设备搜索
	void   SearchCTCamera (HWND hWnd);//摄像机搜索 挂接WM_RECV_DATA 消息获得搜索数据 数据在wParam里包含一个完整的_STR_PACKET_包
	void   CloseSearch();
	void   SearchWG200X   (HWND hWnd);
	//设备升级
	int    UpdateCTCamera     (CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg);
	int    UpdateCTCameraMTD  (CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg=NULL,CStatic *pStatic=NULL);
    
	//恢复设备默认配置
	BOOL RestoreDevice(char *cMac);
	//网络参数设置
	INT  SetNetparam  (USHORT ntype,char* cIP,char* cMask,char* cGateway,USHORT port,char* cMac,char*hostname);
    //网络升级
	void UpdateDevice (long nType, CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg,CStatic*pStatic);

private:
	//CUDPSocket		*m_pSocket;
	//CUDPSendSocket	*m_pSendSocket;
	sockaddr_in      sin;
    SOCKET      	ServerSock;
	
	SOCKET			m_sockSearch;
	ULONG			m_ThreadId;
	HANDLE			m_hThread;
	HWND            m_hWndMsg;

private:
	void SearchCTCameraII(HWND hWnd);
	static DWORD __stdcall RecvThread(void* pParam);
};

#endif // _SEARCHDEVICE_H
