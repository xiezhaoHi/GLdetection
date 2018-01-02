
// GLdetectionDlg.h : ͷ�ļ�
//

#pragma once
#include "mscomm.h"
#include "afxwin.h"
#include "afxcmn.h"

#include "LayoutManager.h"
// CGLdetectionDlg �Ի���
#include "skin/SkinWndHelper.h"

//���ܻ��������� ���size
#define	 REV_BUFF_MAX_SIZE 8*2*6*10*10

class CGLdetectionDlg : public CDialogEx
{
	
// ����
public:
	CGLdetectionDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CGLdetectionDlg();
// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLDETECTION_DIALOG };
#endif

public:
	/*
	Init ��ˢ
	*/
	HBRUSH	m_picture;//����
	HANDLE	m_threadH, m_threadS;
	CFont m_staticFont;
	CFont m_staticTFont;
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

	bool ShowListData(CString const& strData);
// ʵ��
public:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CEvent	m_waitEvent;
	/*���� ���ݰ�*/
	CStringArray	m_strAryJTData;
	//������ݵ�listctrl 
	BOOL  ChangeListData(CStringArray const&);
	// //com ͨ�ſؼ�
	HANDLE	m_hand; //com�ļ����
 	BOOL	m_stopFlag; //�߳̽�����־
	BOOL	m_stopSearch; //���������߳�
	HANDLE InitCom(CString const& comName);
	bool ReadData(HANDLE &handler, char* buffer, int &len);
	bool WriteData(HANDLE &handler, char* buffer, int len);
	//���ܻ�����
	BYTE	m_recBuff[REV_BUFF_MAX_SIZE];
	//��ǰbuffsize
	size_t	m_buffSize;
	CMscomm m_mscomm_ocx;
	CComboBox m_combox_com;
	CString		m_strText;
	Win32Layout::Box m_layout;
	CListCtrl m_listctrl_show;
	CStringArray	m_strAryPrevious;// ����ǰһ�� �յ�������(��������)
									 /*�ǿͻ������Ʊ�����*/
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

	/*��ʼ����������*/
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
