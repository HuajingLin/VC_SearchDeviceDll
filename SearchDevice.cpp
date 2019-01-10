// SearchDevice.cpp: implementation of the CSearchDevice class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"

#include "SearchDevice.h"
#include "UDPSocket.h"
#include "UDPSendSocket.h"
#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CSearchDevice::CSearchDevice()
{
	m_ThreadId		= 0;
	m_sockSearch	= 0;
	m_hThread		= NULL;
	m_hWndMsg       = NULL;
}

CSearchDevice::~CSearchDevice()
{
	if(m_sockSearch)
	{
		closesocket(m_sockSearch);
		m_sockSearch = 0;
	}
	
	if (WaitForSingleObject(m_hThread, 500) == WAIT_TIMEOUT)
	{
		TerminateThread(m_hThread, m_ThreadId);
		CloseHandle(m_hThread);
		m_hThread = INVALID_HANDLE_VALUE;
	}
}

typedef struct 
{
	unsigned int devNo;
	unsigned char macaddr[24];
	unsigned char devregcode[26];
}REGSET;

void CSearchDevice::SearchCTCamera(HWND hWnd)
{
	SearchCTCameraII(hWnd);
}

void CSearchDevice::CloseSearch()
{
	//closesocket(m_sockSearch);
	//m_sockSearch = 0;
}

void CSearchDevice::SearchCTCameraII(HWND hWnd)
{
	WSADATA wsaData;
	SOCKADDR_IN addsin,saUdpServ;
	BOOL  fBroadcast = TRUE;
	int   nSize;
	int   ncount=0;
	SOCKET   SendSocket;
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{   
		MessageBox(NULL, "WSAStartup ERR", NULL, MB_OK);
		return;
	}
	m_hWndMsg=hWnd;
	SendSocket = socket(PF_INET, SOCK_DGRAM, 0);   

	addsin.sin_family = AF_INET;   
	addsin.sin_port   = htons(UDP_PORT_SEND);   
	addsin.sin_addr.s_addr = htonl(INADDR_ANY);//   
	setsockopt(SendSocket, SOL_SOCKET, SO_BROADCAST, (CHAR*)&fBroadcast, sizeof(BOOL));   

	saUdpServ.sin_family  =   AF_INET;
	saUdpServ.sin_addr.s_addr = htonl(INADDR_BROADCAST);   
	saUdpServ.sin_port = htons(UDP_PORT_SEND);   
	nSize = sizeof( SOCKADDR_IN );
	_STR_PACKET_	sp;
	memset(&sp, 0, sizeof(_STR_PACKET_));
	sp.sync		= __start_code__;
	sp.msg_type = MSG_SEARCHQUERY_REQ;
	int n = sendto(SendSocket, (char*)&sp, sizeof(_STR_PACKET_), 0, (SOCKADDR *)&saUdpServ, nSize);
	closesocket(SendSocket);
	if(n <= 0)
		MessageBox(NULL, "send error", NULL, MB_OK);

	if(m_hThread)
	{
		if (WaitForSingleObject(m_hThread, 500) == WAIT_TIMEOUT)
		{
			TerminateThread(m_hThread, m_ThreadId);
			CloseHandle(m_hThread);
			m_hThread = INVALID_HANDLE_VALUE;
			MessageBox(NULL, "TerminateThread", NULL, MB_OK);
		}
	}
	if(!m_sockSearch)
	{
		m_sockSearch = socket(PF_INET, SOCK_DGRAM, 0);   
		addsin.sin_family = AF_INET;   
		addsin.sin_port   = htons(UDP_PORT_READ);   
		addsin.sin_addr.s_addr = htonl(INADDR_ANY);//   
		if(bind(m_sockSearch, (SOCKADDR*)&addsin, sizeof(addsin))!=0)   
		{   
			MessageBox(NULL, "bind ERR", NULL, MB_OK);
			return;   
		}
	}
	m_hThread = CreateThread( NULL, 0, RecvThread, this, 0, &m_ThreadId );
	if(m_hThread == NULL)
	{
		MessageBox(NULL, "Can't Create RecvThread", NULL, MB_OK);
	}
}

DWORD __stdcall CSearchDevice::RecvThread( void* pParam )
{	
	CSearchDevice*     pDlg = ( CSearchDevice* )pParam;
	HWND               hSendtoWnd =  pDlg->m_hWndMsg;
	int                ret;

    fd_set              fdSet;
	timeval             tmval;
	struct sockaddr_in  addfrom;
	int                 addfromLen=sizeof(addfrom);

	_STR_PACKET_        *sp;
	char                RecBuf[500];
	sp                 =(_STR_PACKET_*)RecBuf;

	tmval.tv_sec=2;
	tmval.tv_usec=0;
	
	while (TRUE)
	{
        FD_ZERO(&fdSet);
		FD_SET (pDlg->m_sockSearch, &fdSet);

		ret=select(NULL,&fdSet,NULL,NULL,&tmval);
		if(0 >= ret) break;
		
		ret= recvfrom(pDlg->m_sockSearch, (char*)sp,500,0,(struct sockaddr*)&addfrom, &addfromLen);
		if(0 >= ret) continue;
        
		if(sp->sync == __start_code__ && sp->msg_type == MSG_SEARCHQUERY_RESP)
			SendMessage(hSendtoWnd, WM_RECV_DATA, (WPARAM)sp, NULL);
	}
    
	return 0;
}

void CSearchDevice::SearchWG200X(HWND hWnd)
{
//	DoorInitialize(hWnd);
//	SearchWGDoorDevice();
}
/*
void CSearchDevice::UpdateDevice(long nType, CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg,CStatic*pStatic)
{
	int type =0;
	switch(nType)
	{
	case CT_NVS800:
		if(nFileType==2 ||//Linux内核镜像
		   nFileType==5 ||//运用程序镜像
		   nFileType==6 ){ //配置（注册)镜像
			if(nFileType==2) type=0; //对应设备MTD 升级文件
			if(nFileType==5) type=1;
			if(nFileType==6) type=2;
		   UpdateCTCameraMTD(UpdataIpAddr, strFile, nFileType, type, pProg,pStatic);
		}else
		   UpdateCTCamera(UpdataIpAddr, strFile, nFileType, Reboot, pProg);
		break;
	case WG_200X:
		break;
	default:
		break;
	}
}
*/
int CSearchDevice::UpdateCTCameraMTD(CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg,CStatic*pStatic)
{
	char	IpAddr[60] = {"\0"};
	char	sPort[10] = {"\0"};
	int		iPort = 0;
	T_Update	update;
	FILE	*fp = NULL;
	unsigned char	*imsi_byte =(unsigned char *) &update;
	CString  staText;

//	m_addr.GetWindowText(IpAddr, 59);
//	m_port.GetWindowText(sPort, 9);
//	sscanf(sPort, "%d", &iPort);
	strncpy(IpAddr,UpdataIpAddr,60);
	iPort =40001;
	memset(sin.sin_zero, 0, sizeof(sin.sin_zero));
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(IpAddr);
	sin.sin_port = htons(iPort);

	int ret = 0;

	ServerSock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (ServerSock ==INVALID_SOCKET){
	//	AfxMessageBox(CString(MAKEINTRESOURCE(IDS_NETCONNECTEERROR))); 
		closesocket(ServerSock); 
		return FALSE; 
	}

	if ( (ret=connect(ServerSock, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) == -1){
		ret = GetLastError();
		staText.Format(("Error:Connect %s ",IpAddr));
//		AfxMessageBox(CString(MAKEINTRESOURCE(IDS_NETCONNECTEERROR))); 
		goto Exit_Sub;
	}else{
		//Send user login information to server
		USER_REQ	ur;
		ur.sync = 0x53415443;
		ur.msg_type = 0x00010001;
		//strcpy((char *)ur.user_name, "SantachI");
		//strcpy((char *)ur.password , "IhcatnaS");
		//send(ServerSock, (char *)&ur, sizeof(ur), 0);
        Sleep(50);
		if ( (fp=fopen(strFile, "rb"))==NULL)
		{
			goto Exit_Sub;
		}
		else
		{
			int	dwReadLen = 0;
			int	dwReadTotal = 0;
			int	dwSendByte = 0;
			int	dwSendTotal = 0;
			int	dwSendPacket = 0;
			int	dwFileSize = 0;
			char szTitle[100] = {"\0"};

			dwFileSize = fseek(fp, 0, SEEK_END);

			dwFileSize = ftell(fp);

			fseek(fp, 0, SEEK_SET);
			
			pProg->SetRange32(0, dwFileSize);

		//	GetDlgItem(IDC_TIP)->SetWindowText("");
		//	GetDlgItem(IDC_TIP2)->SetWindowText("");
			do{
		//		m_start.EnableWindow(false);
		//		m_cancel.EnableWindow(false);
				dwReadTotal += dwReadLen = fread(update.file_data, 1, 1400, fp);
				update.file_len = dwFileSize;
				update.data_len = dwReadLen;
				update.msg_type = 0x000E0001;
				update.sub_type = nFileType;//m_type.GetCurSel();
				update.sync = 0x53415443;
				update.err_code = Reboot;
				dwSendPacket = 0;

				while( dwSendPacket<sizeof(T_Update))
				{
					dwSendByte = send(ServerSock,(const char *)imsi_byte + dwSendPacket, 
							sizeof(T_Update) - dwSendPacket, 0);

					if (dwSendByte<0)
					{
						goto Exit_Sub;
					}
					dwSendTotal += dwSendPacket += dwSendByte;
				}//end wile	
				pProg->SetPos(dwSendTotal);
				pProg->UpdateData(true);
				pProg->UpdateWindow();
		//		staText.Format(IDS_SENDMSG,dwReadTotal,(float)dwReadTotal / 1024.0, 
		//					(float)dwReadTotal / (1024.0 * 1024.0));
				sprintf(szTitle, "Sending:%d bytes, %f KiloBytes, %f MBytes.", 
							dwReadTotal,
							(float)dwReadTotal / 1024.0, 
							(float)dwReadTotal / (1024.0 * 1024.0));
			pStatic->SetWindowText(szTitle);
			::SleepEx(50, 0);
			}while(dwReadLen >0);
				sprintf(szTitle, "Sending:%d bytes, %f KiloBytes, %f MBytes.", 
							dwReadTotal,
							(float)dwReadTotal / 1024.0, 
							(float)dwReadTotal / (1024.0 * 1024.0));
		pStatic->SetWindowText(szTitle);
			T_Update_Resp	tur;
		pStatic->SetWindowText("waiting for writting...");

			recv(ServerSock, (char *)&tur, sizeof(tur), 0);

		//	m_start.EnableWindow(true);
	//		m_cancel.EnableWindow(true);

	   // pStatic->SetWindowText(_T("服务器完成刷写FLASH，请按提示进行操作..."));
			if (tur.err_code)
			{
				AfxMessageBox("UpDate False "/*CString(MAKEINTRESOURCE(IDS_UPDATAFAULT))*/);
			}
			else
			{
				//if (AutoReboot)
				//	this->RebootRemote();
				AfxMessageBox("Update OK "/*CString(MAKEINTRESOURCE(IDS_UPDATEOK))*/);
			}
			if (fp)	
			{
				fclose(fp);
			}
			if (ServerSock)
			{
				closesocket(ServerSock);
			}
		}
	}
Exit_Sub:
	if (fp)	fclose(fp);
	if (ServerSock) closesocket(ServerSock);
//	FileName.ReleaseBuffer();	
	return TRUE;
}
int CSearchDevice::UpdateCTCamera(CString UpdataIpAddr, CString strFile, long nFileType, BYTE Reboot, CProgressCtrl *pProg)
{

	char			chCurrentIpAddr[60] = {"\0"};
	int				iPort = 40005;
	T_Update		update;
	FILE			*fp = NULL;
	sockaddr_in		sin ;
	SOCKET			ServerSock;
	unsigned char	*imsi_byte =(unsigned char *) &update;
	int				iType = 0;
	int				ret = 0;
	int             j = 0;
	
	strncpy(chCurrentIpAddr, UpdataIpAddr.GetBuffer(1), 59);
	UpdataIpAddr.ReleaseBuffer();
	
	
	sin.sin_family = AF_INET;
	sin.sin_addr.s_addr = inet_addr(chCurrentIpAddr);
	sin.sin_port = htons(iPort);
	memset(sin.sin_zero, 0, sizeof(sin.sin_zero));
	
	ServerSock = socket (AF_INET, SOCK_STREAM, IPPROTO_TCP); 
	if (ServerSock ==INVALID_SOCKET){
//		AfxMessageBox(CString(MAKEINTRESOURCE(IDS_NETCONNECTEERROR))); 
		closesocket(ServerSock); 
		return -1; 
	}
		
	if ( (ret=connect(ServerSock, (struct sockaddr*)&sin, sizeof(struct sockaddr_in))) == -1){
		ret = GetLastError();
	//	AfxMessageBox(CString(MAKEINTRESOURCE(IDS_NETCONNECTEERROR))); 
		goto Exit_Sub;
	}else{

		USER_REQ	ur;
		ur.sync = 0x43545653;
		ur.msg_type = 0X0E0010007;
		strncpy((char *)ur.user_name, "xiaojun", 15);//升级文件任务连接请求
		strncpy((char *)ur.password , "123456", 15);
	
		if(SOCKET_ERROR == ::send(ServerSock, (char *)&ur, sizeof(ur), 0))
		{
			::closesocket(ServerSock);
			return -1;
		}
	Sleep(100);
        ur.msg_type = 0x00010001;  //
		strncpy((char *)ur.user_name, "xiaojun", 15);//升级文件请求
		strncpy((char *)ur.password , "`162534", 15);
 		send(ServerSock, (char *)&ur, sizeof(ur), 0);
	}
	Sleep(100);
	if( (fp=fopen(strFile.GetBuffer(1), "rb"))==NULL)
	{
		goto Exit_Sub;
	}
	else
	{
		int	dwReadLen = 0;
		int	dwReadTotal = 0;
		int	dwSendByte = 0;
		int	dwSendTotal = 0;
		int	dwSendPacket = 0;
		int	dwFileSize = 0;
		memset(imsi_byte, '0' , sizeof(T_Update));
		//unsigned char	*imsi_byte =(unsigned char *) &update;
		dwFileSize = fseek(fp, 0, SEEK_END);
		dwFileSize = ftell(fp);
		fseek(fp, 0, SEEK_SET);
		pProg->SetPos(0);
		pProg->SetRange32(0, dwFileSize);

		do
		{				
			dwReadTotal += dwReadLen = fread(update.file_data, 1, 1400, fp);
			update.file_len = dwFileSize;
			update.data_len = dwReadLen;
			update.msg_type = 0x00010001;
			update.sub_type = nFileType;
			update.sync = 0x43545653;
			update.err_code = Reboot;
			dwSendPacket = 0;
			while( dwSendPacket<sizeof(T_Update))
			{
				dwSendByte = send(ServerSock, 
					(const char *)imsi_byte + dwSendPacket, 
					sizeof(T_Update) - dwSendPacket, 0);
				if (dwSendByte<0)
				{
					int nErr = WSAGetLastError();
					goto Exit_Sub;
				}
				dwSendTotal += dwSendPacket += dwSendByte;
			}
			pProg->SetPos(dwSendTotal);
			pProg->UpdateData(true);
			pProg->UpdateWindow();
			::SleepEx(50, 0);
		}while(dwReadLen >0);
						
//		T_Update_Resp	tur;
	//	if (recv(ServerSock, (char *)&tur, sizeof(tur), 0)<= 0){
//			goto Exit_Sub;
//		}
		if (fp)	fclose(fp);
		if (ServerSock) closesocket(ServerSock);
//		if (tur.err_code){
//			sprintf(chCurrentIpAddr, "%s:%s:%s", CString(MAKEINTRESOURCE(IDS_STRING32893)), UpdataIpAddr,strFile);
//			AfxMessageBox(_T(chCurrentIpAddr));
//		}
//		else{
//			sprintf(chCurrentIpAddr, "%s:%s:%s", CString(MAKEINTRESOURCE(IDS_STRING32892)), UpdataIpAddr,strFile);
//			AfxMessageBox(_T(chCurrentIpAddr));
//		}
//		AfxMessageBox(CString(MAKEINTRESOURCE(IDS_UPDATEOK)));
		return 1;
	}
	
Exit_Sub:
	if (fp)	fclose(fp);
	if (ServerSock) closesocket(ServerSock);
				
	return 0;

}

INT CSearchDevice::SetNetparam(USHORT ntype, char *cIP, char *cMask, char *cGateway, USHORT port,char *cMac,char*hostname)
{
	_STR_PACKET_ NetPacket;
    strcpy((CHAR*)NetPacket.mac_addr,cMac);
	strcpy((CHAR*)NetPacket.ip_addr ,cIP);
	strcpy((CHAR*)NetPacket.net_mask,cMask);
	strcpy((CHAR*)NetPacket.gw_addr ,cGateway);
	NetPacket.sync =__start_code__;
	NetPacket.port     =port;
	NetPacket.net_type =ntype;
	NetPacket.msg_type =0xFFA5;//Set Net Param
/*
	if(m_pSendSocket == NULL)
	{
		m_pSendSocket	= new CUDPSendSocket;
		m_pSendSocket->SetWnd(NULL);

		if(!m_pSendSocket->Create(UDP_PORT_SEND+1, SOCK_DGRAM))
		{
			AfxMessageBox("Error create send socket!!!");
			delete m_pSendSocket;
			m_pSendSocket = NULL;
			return -1 ;
		}
	}
	m_pSendSocket->SetSockOpt(SO_BROADCAST, &NetPacket, sizeof(_STR_PACKET_));
	m_pSendSocket->SendTo(&NetPacket, sizeof(_STR_PACKET_), UDP_PORT_SEND, "255.255.255.255");
	Sleep(500*1);
*/
	return 0;

}

BOOL CSearchDevice::RestoreDevice(char *cMac)
{
	_STR_PACKET_ NetPacket;
    strcpy((CHAR*)NetPacket.mac_addr,cMac);
	strcpy((CHAR*)NetPacket.ip_addr ,"");
	strcpy((CHAR*)NetPacket.net_mask,"");
	strcpy((CHAR*)NetPacket.gw_addr ,"");
	NetPacket.sync =__start_code__;
	NetPacket.port     =0;
	NetPacket.net_type =0;
	NetPacket.msg_type =0xFFA6;//RESTORE
/*
	if(m_pSendSocket == NULL)
	{
		m_pSendSocket	= new CUDPSendSocket;
		m_pSendSocket->SetWnd(NULL);

		if(!m_pSendSocket->Create(UDP_PORT_SEND, SOCK_DGRAM))
		{
			AfxMessageBox("Error create send socket!!!");
			delete m_pSendSocket;
			m_pSendSocket = NULL;
			return -1 ;
		}
	}
	m_pSendSocket->SetSockOpt(SO_BROADCAST, &NetPacket, sizeof(_STR_PACKET_));
	m_pSendSocket->SendTo(&NetPacket, sizeof(_STR_PACKET_), UDP_PORT_SEND, "255.255.255.255");
	Sleep(1000*5);
*/
	return 0;
}
/*
	WSADATA wsaData;
	SOCKET sockListener;   
	SOCKADDR_IN sin,saUdpServ;   
	BOOL  fBroadcast = TRUE;  
	int   nSize;   
	int   ncount=0;   
	if(WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
	{   
		MessageBox(NULL, "WSAStartup ERR", NULL, MB_OK);
		return;   
	}
	sockListener = socket(PF_INET,SOCK_DGRAM,0);   
	setsockopt(sockListener, SOL_SOCKET, SO_BROADCAST, (CHAR*)&fBroadcast, sizeof(BOOL));   
	sin.sin_family = AF_INET;   
	sin.sin_port   = htons(0);   
	sin.sin_addr.s_addr = htonl(INADDR_ANY);   
	if(bind(sockListener, (SOCKADDR*)&sin, sizeof(sin))!=0)   
	{   
		MessageBox(NULL, "bind ERR", NULL, MB_OK);
		return;   
	}   
	saUdpServ.sin_family  =   AF_INET;   
	saUdpServ.sin_addr.s_addr = htonl(INADDR_BROADCAST);   
	saUdpServ.sin_port = htons(UDP_PORT_SEND);   
	nSize = sizeof( SOCKADDR_IN );
	
	int n = sendto(sockListener, (char*)&sp, sizeof(_STR_PACKET_), 0, (SOCKADDR *)&saUdpServ, nSize);   
*/	
/*
void CSearchDevice::RegCTCamera(HWND hWnd,UINT devNo,CHAR* macaddr,CHAR *devRegcode)
{
    CUDPSocket *pSock=NULL;
    REGSET     *pReg;
	if(pSock == NULL)
	{
		pSock		= new CUDPSocket;
		pSock->SetWnd(hWnd);

		if(!pSock->Create(3322, SOCK_DGRAM))
		{
			AfxMessageBox("Error create socket!!!");
			delete pSock;
			pSock = NULL;
			return ;
		}
	}

	
	_STR_PACKETII_	sp;
	CHAR SetIP[20]="255.255.255.255";
	memset(&sp, 0x00, sizeof(_STR_PACKETII_));
	pReg=(REGSET*)sp.buf;
	sp.sync		= __start_code__;
	sp.msg_type = 0xFFA4;
	pReg->devNo =devNo;
	strncpy((char*)pReg->devregcode,devRegcode,26);
	strncpy((char*)pReg->macaddr   ,macaddr   ,24);
	m_pSendSocket->SetSockOpt(SO_BROADCAST, &sp, sizeof(_STR_PACKETII_));
	m_pSendSocket->SendTo(&sp, sizeof(_STR_PACKETII_), UDP_PORT_SEND, "255.255.255.255");
	::SleepEx(0, 5);
//	m_pSendSocket->SendTo(&sp, sizeof(_STR_PACKETII_), UDP_PORT_SEND, "255.255.255.255");
	::SleepEx(0, 5);
//	m_pSendSocket->SendTo(&sp, sizeof(_STR_PACKETII_), UDP_PORT_SEND, "255.255.255.255");
    
	pSock->Close();
	delete pSock;
	pSock = NULL;

}
*/
/*
void CSearchDevice::SearchDevice(HWND hWnd, long nType)
{
	switch(nType)
	{
	case CT_NVS800:
		SearchCTCamera(hWnd);
		break;
	case WG_200X:
		SearchWG200X(hWnd);
		break;
	default:
		break;
	}
	SetTimer(hWnd, SET_WAIT_TIMER, 2000, NULL);
}
*/