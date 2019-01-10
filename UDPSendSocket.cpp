// UDPSendSocket.cpp : implementation file
//

#include "stdafx.h"
#include "Search.h"
#include "UDPSendSocket.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CUDPSendSocket

CUDPSendSocket::CUDPSendSocket()
{
}

CUDPSendSocket::~CUDPSendSocket()
{
}


// Do not edit the following lines, which are needed by ClassWizard.
#if 0
BEGIN_MESSAGE_MAP(CUDPSendSocket, CSocket)
	//{{AFX_MSG_MAP(CUDPSendSocket)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()
#endif	// 0

/////////////////////////////////////////////////////////////////////////////
// CUDPSendSocket member functions
void CUDPSendSocket::SetWnd(HWND hwnd)
{
	m_pHwnd = hwnd;
}

void CUDPSendSocket::OnClose(int nErrorCode) 
{
	// TODO: Add your specialized code here and/or call the base class
	
	CSocket::OnClose(nErrorCode);
}
