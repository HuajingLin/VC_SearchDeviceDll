// UDPSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Search.h"
#include "UDPSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUDPSocket

CUDPSocket::CUDPSocket()
{
}

CUDPSocket::~CUDPSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUDPSocket, CSocket)
	//{{AFX_MSG_MAP(CUDPSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUDPSocket member functions

void CUDPSocket::SetWnd(HWND hwnd)
{
	m_pHwnd = hwnd;
}

void CUDPSocket::OnReceive(int nErrorCode) 
{
/*
	_STR_PACKET_ *sp = NULL;
	int bufSize = sizeof(_STR_PACKET_)+40;
	char *buffer = new char [bufSize];

	unsigned int	port;
	CString			addr;
//	memset(sp,0,sizeof(_STR_PACKET_));
	int flag = ReceiveFrom(buffer, bufSize, addr, port);
	switch(flag)
	{
	case 0:
//		Close();
		break;
	case SOCKET_ERROR:
		if(GetLastError() != WSAEWOULDBLOCK)
			Close();
		break;
	default:
		sp = (_STR_PACKET_*)buffer;
		if(sp->sync == __start_code__ && sp->msg_type == MSG_SEARCHQUERY_RESP)
		{
			//PostMessage(m_pHwnd, WM_RECV_DATA, (WPARAM)sp, CT_NVS800);
			SendMessage(m_pHwnd, WM_RECV_DATA, (WPARAM)sp, CT_NVS800);
		}
		delete buffer;
		buffer = NULL;
		break;
	}*/
	CSocket::OnReceive(nErrorCode);
}

void CUDPSocket::OnClose(int nErrorCode) 
{
	PostMessage(m_pHwnd, WM_CLOSE_MSG, 0, 0);
	
	CSocket::OnClose(nErrorCode);
}

HWND CUDPSocket::GetWnd()
{
	return m_pHwnd;
}
