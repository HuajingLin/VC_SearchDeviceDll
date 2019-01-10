// stdafx.h : include file for standard system include files,
//  or project specific include files that are used frequently, but
//      are changed infrequently
//

#if !defined(AFX_STDAFX_H__25F65CBF_E3D2_47C9_A815_7BA5D8ACD35D__INCLUDED_)
#define AFX_STDAFX_H__25F65CBF_E3D2_47C9_A815_7BA5D8ACD35D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define VC_EXTRALEAN		// Exclude rarely-used stuff from Windows headers

#include <afxwin.h>         // MFC core and standard components
#include <afxext.h>         // MFC extensions

#ifndef _AFX_NO_OLE_SUPPORT
#include <afxole.h>         // MFC OLE classes
#include <afxodlgs.h>       // MFC OLE dialog classes
#include <afxdisp.h>        // MFC Automation classes
#endif // _AFX_NO_OLE_SUPPORT


#ifndef _AFX_NO_DB_SUPPORT
#include <afxdb.h>			// MFC ODBC database classes
#endif // _AFX_NO_DB_SUPPORT

#ifndef _AFX_NO_DAO_SUPPORT
#include <afxdao.h>			// MFC DAO database classes
#endif // _AFX_NO_DAO_SUPPORT

#include <afxdtctl.h>		// MFC support for Internet Explorer 4 Common Controls
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>			// MFC support for Windows Common Controls
#endif // _AFX_NO_AFXCMN_SUPPORT
#include <afxsock.h>		// MFC socket extensions
#include <Afxtempl.h>
#include <list>
#include <map>
#include <vector>
#include <set>
#include <afxsock.h>		// MFC socket extensions
using namespace std;
#define  WM_CLOSE_MSG   WM_USER+1503
#define  WM_RECV_DATA   WM_USER+1502
enum DEVICE_MODEL { CT_NVS800, WG_200X }; 
typedef struct tagFILE_ATTRIBUTE
{
	CString strFile;
	CString strCamNo;
	CString strRecType;
	CString strDate;
	CString strTime;
	CString strSize;
	CString strExt;
	DWORD dwTime;
}FILE_ATTRIBUTE, *PFILE_ATTRIBUTE;

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STDAFX_H__25F65CBF_E3D2_47C9_A815_7BA5D8ACD35D__INCLUDED_)
