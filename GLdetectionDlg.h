
// GLdetectionDlg.h : 头文件
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "LayoutManager.h"
// CGLdetectionDlg 对话框
#include "skin/SkinWndHelper.h"

//接受缓冲区数据 最大size
#define	 REV_BUFF_MAX_SIZE 8*2*6*10*10

class CGLdetectionDlg : public CDialogEx
{
	
// 构造
public:
	CGLdetectionDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CGLdetectionDlg();
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLDETECTION_DIALOG };
#endif

public:
	/*
	Init 画刷
	*/
	HBRUSH	m_picture;//背景
	HANDLE	m_threadH, m_threadS;
	CFont m_staticFont;
	CFont m_staticTFont;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

	bool ShowListData(CString const& strData);
// 实现
public:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEvent	m_waitEvent;
	/*监听 数据包*/
	CStringArray	m_strAryJTData;
	//填充数据到listctrl 
	BOOL  ChangeListData(CStringArray const&);
	// //com 通信控件
	HANDLE	m_hand; //com文件句柄
 	BOOL	m_stopFlag; //线程结束标志
	BOOL	m_stopSearch; //结束搜素线程
	HANDLE InitCom(CString const& comName);
	bool ReadData(HANDLE &handler, char* buffer, int &len);
	bool WriteData(HANDLE &handler, char* buffer, int len);
	//接受缓冲区
	BYTE	m_recBuff[REV_BUFF_MAX_SIZE];
	//当前buffsize
	size_t	m_buffSize;
	CMscomm m_mscomm_ocx;
	CComboBox m_combox_com;
	CString		m_strText;
	Win32Layout::Box m_layout;
	CListCtrl m_listctrl_show;
	CStringArray	m_strAryPrevious;// 保持前一次 收到的数据(解析过的)
									 /*非客户区绘制标题栏*/
	CSkinWndHelper m_SkinWndHelper;

	DECLARE_EVENTSINK_MAP()

	afx_msg void OnBnClickedButtonStart();
	afx_msg void OnSizing(UINT fwSide, LPRECT pRect);
	afx_msg void OnSize(UINT nType, int cx, int cy);

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);

	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	afx_msg void OnGetMinMaxInfo(MINMAXINFO* lpMMI);

	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp);

	/*初始化界面数据*/
	void InitFaceData(void);

	
	void ReSize();
	POINT old;

	LONG m_nWidth;
	LONG m_nHeight;
	CString m_strBTL;
	CString m_strSJW;
	CString m_strJYW;
	CString m_strTZW;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
protected:
	afx_msg LRESULT OnShowdata(WPARAM wParam, LPARAM lParam);
public:
	CStatic m_ctrlPic;
	afx_msg void OnClose();
	
	CStatic m_static_pic2;
};
