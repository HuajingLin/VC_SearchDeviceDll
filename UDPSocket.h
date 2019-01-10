#if !defined(AFX_UDPSOCKET_H__E6A9CE01_A8A5_42EA_A9B9_D457B68FAF37__INCLUDED_)
#define AFX_UDPSOCKET_H__E6A9CE01_A8A5_42EA_A9B9_D457B68FAF37__INCLUDED_


#pragma once

// UDPSocket.h : header file
//


typedef unsigned __int32	uint32_t;

const int __start_code__ = ((uint32_t)0x43545653);//((uint32_t)0x53415443);
const int MSG_SEARCHQUERY_REQ = 0x000E0003;			//搜索设备请求	
const int MSG_SEARCHQUERY_RESP = 0x000E0004;		//搜索设备回应	
const int MSG_USERLOGIN_REQ = 0x00010001;			//用户登陆请求
const int MSG_NETPARMSET_REQ = 0x000D0001;			//网络参数设置请求	

const int MAX_RECV_BUFFER = 100000;
const int SET_WAIT_TIMER = 1010;
const int UDP_PORT_SEND = 40011;
const int UDP_PORT_READ = 40012;

typedef struct{
	unsigned __int32 sync;
	unsigned __int32 msg_type;
	unsigned __int32 sub_type;
	unsigned __int32 len;
	unsigned __int32 err_code;

	unsigned char	 file_data[1400];
	unsigned __int32 data_len;
	unsigned __int32 file_len;
	unsigned __int32 CRC;
}T_Update;

typedef struct{
	uint32_t	sync;		//0x53415443--'S', 'A', 'T', 'C',synchronize fields
	uint32_t	msg_type;	//data packet type field
	uint32_t	sub_type;	//data packet sub type field
	uint32_t	len;		//data domain valid len
	uint32_t	err_code;	//err code;

	unsigned char	user_name[16];		//user name
	unsigned char	password[16];		//password
	time_t			local_time;		//time check
	uint32_t		subscr_flag;		//subscrible log event? 0-yes, other is no

	unsigned char	buf[200 - (20+ 16 + 16+ 4 + 4)];
}USER_REQ;

typedef struct{
	uint32_t	sync;		//0x53415443--'S', 'A', 'T', 'C',synchronize fields
	uint32_t	msg_type;	//data packet type field
	uint32_t	sub_type;	//data packet sub type field
	uint32_t	len;		//data domain valid len
	uint32_t	err_code;	//err code;
	unsigned char	buf[200 - 20];
}T_Update_Resp;

typedef struct __str_packet__{
	uint32_t	sync;		//0x53415443----'S', 'A', 'T', 'C'  //synchronize fields
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

typedef struct __str_packetII__{
	uint32_t	sync;		//0x53415443----'S', 'A', 'T', 'C'  //synchronize fields
	uint32_t	msg_type;	//data packet type field
	uint32_t	sub_type;	//data packet sub type field
	uint32_t	len;		//data domain valid len
	uint32_t	err_code;	//err code;


	unsigned char	buf[200-20];
}_STR_PACKETII_;
/////////////////////////////////////////////////////////////////////////////
// CUDPSocket command target

class CUDPSocket : public CSocket
{
// Attributes
public:
	HWND	m_pHwnd;

// Operations
public:
	CUDPSocket();
	virtual ~CUDPSocket();

// Overrides
public:
	HWND GetWnd();
	void SetWnd(HWND hwnd);
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CUDPSocket)
	public:
	virtual void OnReceive(int nErrorCode);
	virtual void OnClose(int nErrorCode);
	//}}AFX_VIRTUAL

	// Generated message map functions
	//{{AFX_MSG(CUDPSocket)
		// NOTE - the ClassWizard will add and remove member functions here.
	//}}AFX_MSG

// Implementation
protected:
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_UDPSOCKET_H__E6A9CE01_A8A5_42EA_A9B9_D457B68FAF37__INCLUDED_)
