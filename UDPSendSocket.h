#if !defined(AFX_UDPSENDSOCKET_H__E926238A_42E3_40DC_B0B0_6917E609C395__INCLUDED_)
#define AFX_UDPSENDSOCKET_H__E926238A_42E3_40DC_B0B0_6917E609C395__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// UDPSendSocket.h : header file
//



/////////////////////////////////////////////////////////////////////////////
// CUDPSendSocket command target

class CUDPSendSocket : public CSocket
//class CUDPSendSocket : public CAsyncSocket
{
// Attributes
public:
	HWND	m_pHwnd;
// Operations
public:
	CUDPSendSocket();
	virtual ~CUDPSendSocket();

// Overrides
public:
	void SetWnd(HWND hwnd);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPSendSocket)
	public:
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUDPSendSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPSENDSOCKET_H__E926238A_42E3_40DC_B0B0_6917E609C395__INCLUDED_)
