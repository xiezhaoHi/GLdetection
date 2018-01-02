
// GLdetectionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "GLdetection.h"
#include "GLdetectionDlg.h"
#include "afxdialogex.h"
#include "ReadWriteConfig\ReadWriteConfig.h"
#include "logrecord\LogRecord.h"
#include "public\publicFun.h"
#include "resource.h"
#include "LayoutManager.h"
#include "Change_skin.h"
#include "crc.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#define WM_SHOWDATA WM_USER + 222
#define WAIT_COM_TIME 5000 //����
#define WAIT_SEARCH_TIME 3000 //����  �ȴ�����ʱ��
// CGLdetectionDlg �Ի���

#define VERSION_1010 //�°汾

//��ʶ
#ifdef VERSION_1010
static CString gStrFlag = _T("7B040C");
#else
static CString gStrFlag = _T("0B10");
#endif

// LISTSHOW_SO2 = 0,
// LISTSHOW_NO,
// LISTSHOW_CO,
// LISTSHOW_NO2,
// LISTSHOW_O2,
// LISTSHOW_CO2,
static int idToControlId[] = { LISTSHOW_O2 ,LISTSHOW_CO
,LISTSHOW_NO ,LISTSHOW_SO2,LISTSHOW_NO2 ,LISTSHOW_CO2 ,-1 };

/*���������ĺ�*/
#define  SHOW_MOUDLE
static int controlID[] = { IDC_STATIC_SO2,IDC_STATIC_NO,IDC_STATIC_CO,IDC_STATIC_NO2
,IDC_STATIC_O2,IDC_STATIC_CO2,-1 };
static int controlIDT[] = { IDC_STATIC_SO2_T,IDC_STATIC_NO_T,IDC_STATIC_CO_T,IDC_STATIC_NO2_T
,IDC_STATIC_O2_T,IDC_STATIC_CO2_T,-1 };
static int controlIDG[] = { -1,IDC_STATIC_ONE,IDC_STATIC_THREE,IDC_STATIC_TWO,IDC_STATIC_FOUR,-1 };


static int staticID[] = { IDC_STATIC_CK,IDC_STATIC_BTL,IDC_STATIC_SJW,IDC_STATIC_JYW
,IDC_STATIC_TZW,-1 };
static int staticIDT[] = { -1,IDC_STATIC_BTL_TEXT,IDC_STATIC_SJW_TEXT,IDC_STATIC_JYW_TEXT
,IDC_STATIC_TZW_TEXT,-1 };

CGLdetectionDlg::CGLdetectionDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_GLDETECTION_DIALOG, pParent)
	, m_strBTL(_T(""))
	, m_strSJW(_T(""))
	, m_strJYW(_T(""))
	, m_strTZW(_T(""))
	, m_stopFlag(TRUE)
	, m_stopSearch(TRUE)
	,m_hand (NULL)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

	memset(m_recBuff, 0, REV_BUFF_MAX_SIZE);// ��ʼ��
	m_buffSize = 0;
	//m_waitEvent = CreateEvent()
	
	m_threadS = m_threadH = NULL;
}





CGLdetectionDlg::~CGLdetectionDlg()
{
	
	m_SkinWndHelper.Detach();
	m_stopFlag = false; //�ͷ��߳�
	m_stopSearch = false;

}

void CGLdetectionDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	//DDX_Control(pDX, IDC_MSCOMM, m_mscomm_ocx);

	DDX_Control(pDX, IDC_COMBO_COM, m_combox_com);

	DDX_Text(pDX, IDC_STATIC_BTL_TEXT, m_strBTL);
	DDX_Text(pDX, IDC_STATIC_SJW_TEXT, m_strSJW);
	DDX_Text(pDX, IDC_STATIC_JYW_TEXT, m_strJYW);
	DDX_Text(pDX, IDC_STATIC_TZW_TEXT, m_strTZW);
	DDX_Control(pDX, IDC_STATIC_pic, m_ctrlPic);
	DDX_Control(pDX, IDC_STATIC_pic2, m_static_pic2);
}

BEGIN_MESSAGE_MAP(CGLdetectionDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_BUTTON_START, &CGLdetectionDlg::OnBnClickedButtonStart)
	ON_WM_SIZING()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_GETMINMAXINFO()
	ON_WM_ERASEBKGND()
	ON_WM_NCCALCSIZE()
	ON_MESSAGE(WM_SHOWDATA, &CGLdetectionDlg::OnShowdata)
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CGLdetectionDlg ��Ϣ�������

/*list �ؼ�����Ҫ��ʾ������*/
TCHAR* chListShow[] = {
	_TEXT("��������(ppm)"),
	_TEXT("һ������(ppm)"),
	_TEXT("һ����̼(ppm)"),
	_TEXT("��������(ppm)"),
	_TEXT("����(%)"),
	_TEXT("������̼(%)"),
	NULL
};

// {0x0B, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},			//O2
// { 0x0B, 0x10, 0x03, 0x00, 0x00, 0x00, 0x00, 0x00 },			//CO
// { 0x0B, 0x10, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00 },			//NO
// { 0x0B, 0x10, 0x07, 0x00, 0x00, 0x00, 0x00, 0x00 },			//SO2
// { 0x0B, 0x10, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00 },			//NOX
//																		{0x0B, 0x10, 0x09, 0x00, 0x00, 0x00, 0x00, 0x00},		// H2
// { 0x0B, 0x10, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00 }			//CO2
TCHAR* chCOMInstruct[] = {
	_T(""),//_TEXT("0B10070000000000"),
	_TEXT("0B10040000000000"),
	_TEXT("0B10030000000000"),
	_TEXT("0B10060000000000"),
	_TEXT("0B10000000000000"),
	NULL,//_TEXT("0B10010000000000"),
	NULL
};

char* sendBuff[] = {
	"",//"0B10070000000000",
	"0B10040000000000",
	"0B10030000000000",
	"0B10060000000000",
	"0B10000000000000",
	"",//"0B10010000000000",
	NULL
};


static CStringArray strAry;
HANDLE CGLdetectionDlg::InitCom(CString const& comName)
{
	HANDLE hCom;
	hCom = CreateFile(comName,//COM��  
		GENERIC_READ | GENERIC_WRITE, //�������д  
		FILE_SHARE_READ|FILE_SHARE_WRITE, //��ռ��ʽ  
		NULL,
		OPEN_EXISTING, //�򿪶����Ǵ���  
		0, //ͬ����ʽ  
		NULL);
	if (hCom == (HANDLE)-1)
	{
		int err = GetLastError();
		CString temp;
		temp.Format(_T("%d"), err);
		CLogRecord::WriteRecordToFile(CString("202")+ temp);
		
		return NULL;
	}
	SetupComm(hCom, REV_BUFF_MAX_SIZE, REV_BUFF_MAX_SIZE); //���뻺����������������Ĵ�С
	COMMTIMEOUTS TimeOuts;
	//�趨����ʱ  
	TimeOuts.ReadIntervalTimeout = MAXDWORD;
	TimeOuts.ReadTotalTimeoutMultiplier = 0;
	TimeOuts.ReadTotalTimeoutConstant = 0;
	//�ڶ�һ�����뻺���������ݺ���������������أ�  
	//�������Ƿ������Ҫ����ַ���  
	//�趨д��ʱ  
	//TimeOuts.WriteTotalTimeoutMultiplier = 100;
	//TimeOuts.WriteTotalTimeoutConstant = 500;
	SetCommTimeouts(hCom, &TimeOuts); //���ó�ʱ  

	DCB dcb;
	if (GetCommState(hCom, &dcb))
	{
		//m_strBTL, m_strJYW, m_strSJW, m_strTZW
		//if(m_strBTL.IsEmpty() || m_strJYW.IsEmpty()|| m_strSJW.IsEmpty()|| m_strTZW.IsEmpty())
		dcb.BaudRate = m_strBTL.IsEmpty()?115200: _ttoi(m_strBTL); //������Ϊ9600  
		dcb.ByteSize = m_strSJW.IsEmpty()?8:_ttoi(m_strSJW); //ÿ���ֽ���8λ  
		dcb.Parity = NOPARITY; //����żУ��λ  
		dcb.StopBits = 0; //����ֹͣλ  
		if (!SetCommState(hCom, &dcb))
		{
			int err = GetLastError();
		
			CString temp;
			temp.Format(_T("%d"), err);
			CLogRecord::WriteRecordToFile(temp);
			CloseHandle(hCom);
			return NULL;
		}
	}
	else
	{
		int err = GetLastError();
		CString temp;
		temp.Format(_T("%d"), err);
		CLogRecord::WriteRecordToFile(temp);
		CloseHandle(hCom);
		return NULL;
	}
	
	PurgeComm(hCom, PURGE_TXCLEAR | PURGE_RXCLEAR);
	return hCom;
}



void StrToHex(BYTE *pbDest, char *pbSrc, int nLen)
{
	char h1, h2;
	BYTE s1, s2;
	int i;

	for (i = 0; i < nLen; i++)
	{
		h1 = pbSrc[2 * i];
		h2 = pbSrc[2 * i + 1];

		s1 = toupper(h1) - 0x30;
		if (s1 > 9)
			s1 -= 7;

		s2 = toupper(h2) - 0x30;
		if (s2 > 9)
			s2 -= 7;

		pbDest[i] = s1 * 16 + s2;
	}
}
bool CGLdetectionDlg::WriteData(HANDLE  & handler, char* buffer,int len)
{
		OVERLAPPED m_osWrite;
		memset(&m_osWrite, 0, sizeof(OVERLAPPED));

		m_osWrite.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);
	
	
		//char lpOutBuffer[7];
		//strcpy(lpOutBuffer,'select');
		DWORD dwBytesWrite = len;
		CByteArray hexdata;
						//dwBytesWrite = strAction.GetLength();
		//
		
		BYTE c[8] = { 0 };													 //strAction = 'D8 D2';

		StrToHex(c,buffer, len); //�˴����ص�
		
		//BYTE *c = (BYTE*)hexdata.GetData();
		COMSTAT ComStat;
		DWORD dwErrorFlags;
		BOOL bWriteStat;
		ClearCommError(handler, &dwErrorFlags, &ComStat);
		bWriteStat = WriteFile(handler, c,
			dwBytesWrite, &dwBytesWrite, &m_osWrite);
		if (!bWriteStat)
		{
			if (GetLastError() == ERROR_IO_PENDING)
			{
				WaitForSingleObject(m_osWrite.hEvent, 1000);
			}
		}
		CloseHandle(m_osWrite.hEvent);
		return true;
	
}

/*
��������
*/
bool CGLdetectionDlg::ReadData(HANDLE & handler, char* buffer,int &len)
{
	
	char readBuffer[REV_BUFF_MAX_SIZE];
	memset(readBuffer, 0, REV_BUFF_MAX_SIZE);
	DWORD wCount = REV_BUFF_MAX_SIZE;//��ȡ���ֽ���  
	BOOL bReadStat;

		bReadStat = ReadFile(handler, readBuffer, wCount, &wCount, NULL);
		if (!bReadStat || 0 == wCount)
		{
			return false;
		}
		len = wCount;
		memcpy(buffer, readBuffer, wCount);
		PurgeComm(handler, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
		
	
	return true;
}

DWORD WINAPI find_com(LPVOID lpThreadParameter) //Ѱ�Ҵ򿪵�com��
{
	CGLdetectionDlg* pDlg = (CGLdetectionDlg*)lpThreadParameter;

	CString strCom;
	HANDLE hCom;
	const int	comNum = 9;
	HANDLE	buffCom[comNum+1] = { 0 }; 
	int		buffComID[comNum+1] = { 0 };
	char buffData[REV_BUFF_MAX_SIZE] = { 0 };
	CString	strTemp, strRXData;
	int	lenIndex = -1;
	const int sendBuffLen = 8; //ָ���ֽ���
	int realComId = -1;

	while (pDlg->m_stopSearch)
	{
		for (int index = 1; index <= comNum; ++index)
		{
			strCom.Format(_T("\\\\.\\COM%d"), index);

			hCom = pDlg->InitCom(strCom);
			if (hCom != NULL) //��ʾCOM����Ч
			{
				buffCom[index] = hCom;
				buffComID[index] = index;
			}
		}
		for (int indexX = 1; indexX <= comNum; ++indexX)
		{

			if (buffCom[indexX])
			{
			
				Sleep(WAIT_COM_TIME);
				strRXData = "";
				int retLen = 0;
				if (pDlg->ReadData(buffCom[indexX], buffData, retLen))
				{
					for (int indexY = 0; indexY < retLen; ++indexY)
					{
						strTemp.Format(_T("%02X"), 0xff & buffData[indexY]);
						strRXData += strTemp;
					}
					/*�����������*/
					CLogRecord::WriteRecordToFile(_T("___����COM��ʱ,У���������Ϊ:") + strRXData);

					int retIndex = strRXData.Find(gStrFlag);
					if (retIndex != -1)
					{
						realComId = indexX;
						break;
					}
				}
				//Sleep(WAIT_COM_TIME);
			}
		}
		for (int indexX = 1; indexX <= comNum; ++indexX)
		{
			CloseHandle(buffCom[indexX]); //�ر�
			buffCom[indexX] = NULL;
		}

		if (realComId != -1)
		{
			
			pDlg->m_combox_com.SetCurSel(realComId - 1);
			pDlg->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START, BN_CLICKED), NULL);
			realComId = -1;
			SuspendThread(pDlg->m_threadS);
		}
		Sleep(WAIT_SEARCH_TIME);
	}
	//pDlg->OnBnClickedButtonStart();
	//pDlg->PostMessage(IDC_BUTTON_START, 0, 0);
	
	//((CButton*)pDlg->GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("��������!"));
		return -1;
}


//��ȡ�ظ����Ӵ�����
static int getFun(LPCTSTR str, LPCTSTR substr) {
	int s = 0, lsub = lstrlen(substr);
	while (str = StrStrI(str, substr))
		s++, str += lsub;
	return s;
}


/*���������������ǽ��������е�����Ԫ�ص�λ��*/
void swap(CStringArray & array, int i, int j)
{
	CString tmp = array[i];
	array[i] = array[j];
	array[j] = tmp;
}

/*��������*/
void InsertSort(CStringArray & array, int n)
{
	for (int i = 1; i<n; i++)
	{
		for (int j = i; j>0; j--)
		{
			if (array[j].Mid(4, 2)>array[j - 1].Mid(4, 2))
				swap(array, j, j - 1);
			else
				break;
		}
	}
}

//�м�ֵ�˲�
CString getRightData(CString const& strData)
{
	CString strTemp = strData;
	int size = getFun(strTemp, _T("0B10"));
	if (size <= 0)
		return _T("");
	int beg = -1;
	//CString* list = new CString[size];
	CStringArray list;
	
	const int strLength = 16;
	CString resTemp;
	for (int index = 0; index < size && strTemp.GetLength()>= strLength; ++index)
	{
		beg = strTemp.Find(_T("0B10"));
		if (beg != -1)
		{
			resTemp = strTemp.Mid(beg, strLength);
			if (resTemp.GetLength() == strLength)
			{
				list.Add(resTemp);
			}
			else
			{
				break;
			}

			strTemp = StrStrI(strTemp, _T("0B10"))+ strLength;
			
		}
		else
		{
			break;
		}
	}
	
	//����
	size = list.GetSize();
	if (size<=0)
	{
		return _T("");
	}
	InsertSort(list, size);
	int real = size / 2;
	
	CString strRet = list[real];
	if (strRet.GetLength() == strLength)
	{
		return strRet;
	}
	else
	{
		return _T("");
	}
}

//������� �����߳�
DWORD WINAPI pthread_fun(LPVOID lpThreadParameter)
{
	CGLdetectionDlg* pDlg = (CGLdetectionDlg*)lpThreadParameter;
	char buff[REV_BUFF_MAX_SIZE] = { 0 };
	CString strTemp, strRXData;
	int lenIndex = -1, begIndex = -1, nextIndex = -1;
	int begin = 0, end = 0, result = 0;
	//int num = 0;
	const int sendBuffLen = 8; //ָ���ֽ���
	const int sendMaxCount = 5; //���ʹ���
	int errTimes = 0;
	while (pDlg->m_stopFlag)
	{
	
		errTimes = 0;
			
		while (errTimes < 3)
		{
			Sleep(WAIT_COM_TIME);
			strRXData = _T("");
			memset(buff, 0, REV_BUFF_MAX_SIZE); //��ʼ�� ����
			int readlen = 0;
			if (pDlg->m_hand &&	pDlg->ReadData(pDlg->m_hand, buff, readlen)) //����������
			{
				
				PurgeComm(pDlg->m_hand, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

				for (int index = 0; index < readlen && index < REV_BUFF_MAX_SIZE; ++index)
				{
					strTemp.Format(_T("%02X"), 0xff & buff[index]);
					strRXData += strTemp;
				}
				/*�����������*/
				CLogRecord::WriteRecordToFile(_T("___��������Ϊ:") + strRXData + _T("______��ʼ�����ݽ��������!___"));
				if (!pDlg->ShowListData(strRXData)) //���ص���������
				{
					++errTimes;
				}
				else
				{
					errTimes = 0;
				}
			}
			else
			{
				++errTimes;
				
			}
			
		}
		if (errTimes >=3)
		{
			CloseHandle(pDlg->m_hand);
			pDlg->m_hand = NULL;

			//	pDlg->PostMessage(WM_COMMAND, MAKEWPARAM(IDC_BUTTON_START, BN_CLICKED), NULL);
			pDlg->m_strText = _T("�豸����");
			((CButton*)pDlg->GetDlgItem(IDC_BUTTON_START))->SetWindowText(pDlg->m_strText);
			ResumeThread(pDlg->m_threadS);
			SuspendThread(pDlg->m_threadH);
			errTimes = 0;
			CLogRecord::WriteRecordToFile(_T("_�豸����_�����ȴ�"));
		}
		//int show = num;
		

		}

	return 0;
}

//������
void SetAutoRun(BOOL bAutoRun)
{
	// ������´���
		HKEY   hKey;
	char pFileName[MAX_PATH] = { 0 };
	//�õ����������ȫ·�� 
	DWORD dwRet = GetModuleFileNameW(NULL, (LPWCH)pFileName, MAX_PATH);
	//�ҵ�ϵͳ�������� 
	LPCTSTR lpRun = _T("Software\\Microsoft\\Windows\\CurrentVersion\\Run");
	//��������Key 
	long lRet = RegOpenKeyEx(HKEY_LOCAL_MACHINE, lpRun, 0, KEY_WRITE, &hKey);
	if (lRet == ERROR_SUCCESS)
	{
		
		//RegDeleteKey(hKey, _T("CGLdetectionDlg"));
		//���ע��
		RegSetValueEx(hKey, _T("CGLdetectionDlg"), 0, REG_SZ, (const BYTE*)(LPCSTR)pFileName, MAX_PATH);
		RegCloseKey(hKey);
	}
	//return 0;
}


/*
����
*/
void MYTEST()
{
	CString strRXData;
	CStringArray strAry;
	//1.û��ob10
	strAry.Add(_T("0FF"));
	//2.��0b10 ��ͷ ���ҳ��� ����
	strAry.Add(_T("0B10FFF"));
	//3.�м�
	strAry.Add(_T("FF0B10FFF"));
	//4.��β
	strAry.Add(_T("FFFFFFFFF0B10"));
	
	for (int index = 0; index < strAry.GetSize(); ++index)
	{


	strRXData = getRightData(strAry[index]);
	if (strRXData.IsEmpty())
	{

		continue;
	}
	int strInt = getFun(strRXData.GetString(), _T("0B10"));

	if (strInt > 1) //�����ظ�����
	{
		Sleep(WAIT_COM_TIME);
		continue;
	}
	//strRXData = StrStrI(strRXData, _T("0B10"))+16;
	int begIndex = strRXData.Find(_T("0B10"));
	if (-1 == begIndex)
	{
		Sleep(WAIT_COM_TIME);
		continue;
	}


	CString strTemp;
	


	strTemp = strRXData.Mid(begIndex, 16);
	CLogRecord::WriteRecordToFile(strTemp);
	strTemp = strTemp.Mid(4, 4);
	//���ݼ�� 0000  FFFF 
	if (_T("0000") == strTemp || _T("FFFF") == strTemp)
	{
		continue;
	}

	int end , begin ;
	sscanf_s(CpublicFun::UnicodeToAsc(strTemp.Left(2)), "%x", &begin);
	sscanf_s(CpublicFun::UnicodeToAsc(strTemp.Right(2)), "%x", &end);

	int result = ((end << 8) | begin)&(~0x8000);

	strTemp.Format(_T("%d"), result);
	}
	return;
}
BOOL CGLdetectionDlg::OnInitDialog()
{
// 	int end = 128, begin = 2;
// 	int result = ((end << 8) | begin)&(~0x8000);
// 	long l = (end << 8);
// 	l = (end << 8) | begin;
// 	l = l & (~0x8000);
	CDialogEx::OnInitDialog();

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	//SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	int id = GetCurrentThreadId();
	CRect rect;
	GetClientRect(&rect);     //ȡ�ͻ�����С    
	old.x = rect.right - rect.left;
	old.y = rect.bottom - rect.top;

	SetWindowText(_T("�� ¯ �� �� �� ��"));
#ifdef DEBUGT
	CString t = _T("0B 10 04 00 00 00 00 00");
	for (int index = 0; index < 20; ++index)
	{
		t += t;
	}
	for(int index = 0; index < 10000; ++index)
		getRightData(t);
	return -1;
	MYTEST();
	
#endif

#ifndef DEBUG
	/*��������*/
	SetAutoRun(TRUE);
#endif
	/*�����ʼ��*/
	Change_skin::GetInstance(this)->InitFace();
	InitFaceData();
	//��ֹ�߿�ModifyStyle(0, ); 
	ModifyStyle(m_hWnd, WS_THICKFRAME , 0, 0);
	ModifyStyle(0, WS_MINIMIZEBOX);
#ifdef CSKIN
	m_SkinWndHelper.Attach(m_hWnd,this);
#endif
	//ShowWindow(SW_MAXIMIZE);
	//ShowWindow(SW_NORMAL);
	
	//�ɼ��߳�
	m_threadH = CreateThread(NULL, 0, pthread_fun, this, 0, NULL);
	SuspendThread(m_threadH);

	m_strText = _T("��ʼ���!");
	((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(m_strText);

	//���������߳�
	m_threadS=CreateThread(NULL, 0, find_com, this, 0, NULL); 
	//

	CBitmap bmp;

	bmp.LoadBitmap(IDB_BITMAP1);

	m_picture =  CreatePatternBrush(bmp);

	bmp.DeleteObject();

	((CButton*)GetDlgItem(IDC_STATIC_CO_T))->SetWindowText(_T("PPM"));
	((CButton*)GetDlgItem(IDC_STATIC_NO_T))->SetWindowText(_T("PPM"));
	((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->SetWindowText(_T("PPM"));
	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->SetWindowText(_T("%"));

	((CButton*)GetDlgItem(IDC_STATIC_CO))->SetWindowText(_T("CO"));
	((CButton*)GetDlgItem(IDC_STATIC_NO))->SetWindowText(_T("NO"));
	((CButton*)GetDlgItem(IDC_STATIC_NO2))->SetWindowText(_T("NO2"));
	((CButton*)GetDlgItem(IDC_STATIC_O2))->SetWindowText(_T("O2"));

	
	m_staticFont.CreatePointFont(450, _T("΢���ź�"));
	m_staticTFont.CreatePointFont(225, _T("΢���ź�"));

	((CButton*)GetDlgItem(IDC_STATIC_CO_T))->SetFont(&m_staticTFont);//�������� 
	((CButton*)GetDlgItem(IDC_STATIC_NO_T))->SetFont(&m_staticTFont);//�������� 
	((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->SetFont(&m_staticTFont);//�������� 
	((CButton*)GetDlgItem(IDC_STATIC_O2_T))->SetFont(&m_staticTFont);//�������� 

	((CButton*)GetDlgItem(IDC_STATIC_CO))->SetFont(&m_staticFont);
	((CButton*)GetDlgItem(IDC_STATIC_NO))->SetFont(&m_staticFont);
	((CButton*)GetDlgItem(IDC_STATIC_NO2))->SetFont(&m_staticFont);
	((CButton*)GetDlgItem(IDC_STATIC_O2))->SetFont(&m_staticFont);
	

		((CButton*)GetDlgItem(IDC_STATIC_CO))->SetWindowText(_T("9999"));
		((CButton*)GetDlgItem(IDC_STATIC_NO))->SetWindowText(_T("9999"));
		((CButton*)GetDlgItem(IDC_STATIC_NO2))->SetWindowText(_T("9999"));
		((CButton*)GetDlgItem(IDC_STATIC_O2))->SetWindowText(_T("99.99"));

// 	((CButton*)GetDlgItem(IDC_STATIC_CO_T))->ShowWindow(SW_HIDE);
// 		((CButton*)GetDlgItem(IDC_STATIC_NO_T))->ShowWindow(SW_HIDE);
// 		((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->ShowWindow(SW_HIDE);
// 		((CButton*)GetDlgItem(IDC_STATIC_O2_T))->ShowWindow(SW_HIDE);

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�
#define  BKCOLOR RGB(0xea, 0xec, 0xf1)
void CGLdetectionDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}


	UpdateWindow();
	CRect rect;
	CPaintDC dc(this);
	GetClientRect(rect);
	dc.FillSolidRect(rect, RGB(255, 255, 255));  //��ɫ�����Լ��������

	CClientDC *pDC = new CClientDC(GetDlgItem(IDC_STATIC_LOG));
	CRect rect2;
	GetDlgItem(IDC_STATIC_LOG)->GetWindowRect(&rect2);
	Graphics graphics(pDC->m_hDC); // Create a GDI+ graphics object
	Image image(CLogRecord::GetAppPath() + _T("//picture//logo.png")); // Construct an image

	graphics.DrawImage(&image, 0, 0, image.GetWidth(), image.GetHeight());
	delete pDC;

	return;
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CGLdetectionDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

BEGIN_EVENTSINK_MAP(CGLdetectionDlg, CDialogEx)
	//ON_EVENT(CGLdetectionDlg, IDC_MSCOMM, 1, CGLdetectionDlg::OnCommMscomm, VTS_NONE)
END_EVENTSINK_MAP()


//��ʼ�������������
void CGLdetectionDlg::OnBnClickedButtonStart()
{

	//m_stopSearch = false;
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString temp, strCom;

	m_combox_com.GetWindowText(strCom);

	int index = strCom.Find(_T("COM"));
	if (index < 0)
	{
		return;
	}
	strCom = strCom.Right(strCom.GetLength() - index - 3);

	int size = 10 * 1024;
	//temp.Format("9600,n,8,1");
// 	CString m_strBTL;
// 	CString m_strSJW;
// 	CString m_strJYW;
// 	CString m_strTZW;
	//temp.Format(_T("%s,%s,%s,%s"), m_strBTL, m_strJYW, m_strSJW, m_strTZW);
	strCom.Format(_T("\\\\.\\COM%s"), strCom);

	m_hand = InitCom(strCom);
	if (m_hand)
	{
		//m_stopFlag = true;
		ResumeThread(m_threadH);
		m_strText = _T("��������!");
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(m_strText);

		//((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow();

	}
	else
		ResumeThread(m_threadS);
	return;
	if (m_strText != _T("��������!"))
	{
		
		/*
		else
		{
			((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("���������豸,���Ժ�!"));
			m_strText = _T("���������豸,���Ժ�!");
			((CButton*)GetDlgItem(IDC_BUTTON_START))->EnableWindow(0);
			//���������߳�
			CreateThread(NULL, 0, find_com, this, 0, NULL);
		}
		*/
	}
	else //���� �߳� �� com�ļ����
	{
		//m_stopFlag = false;
		SuspendThread(m_threadH);
		CloseHandle(m_hand);
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("��ʼ����!"));
		m_strText = _T("��ʼ����!");
	}

#ifdef MSCOMM_OCX
	if (m_mscomm_ocx.get_PortOpen())
	{ 
		m_mscomm_ocx.put_PortOpen(FALSE);
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("��ʼ����!"));
		m_strText = _T("��ʼ���");
		return;
	}
		

	m_mscomm_ocx.put_CommPort(_ttoi(strCom)); //���ô���
	m_mscomm_ocx.put_Settings(temp); //���ò����ʵȲ���
	m_mscomm_ocx.put_RThreshold(1); //�����յ����ٸ��ַ��󴥷�OnComm�¼�
	m_mscomm_ocx.put_InBufferSize(size);
	m_mscomm_ocx.put_InputLen(0);
	//m_mscomm_ocx.put_OutBufferSize(size);
	//m_mscomm_ocx.put_OutBufferCount(0);

	//int mode = m_mscomm_ocx.get_InputMode();
	//�ı�ģʽ
	//m_mscomm_ocx.put_InputMode(0);

	//������ģʽ
	m_mscomm_ocx.put_InputMode(1);

	if (!m_mscomm_ocx.get_PortOpen())
	{
		m_mscomm_ocx.put_PortOpen(TRUE); //�򿪴���
		((CButton*)GetDlgItem(IDC_BUTTON_START))->SetWindowText(_T("��������!"));
		m_strText = _T("��������");
		CReadWriteConfig::ConfigSetValue(_T("COM"), _T("COM_CUR"), strCom);
	}
#endif
}


void CGLdetectionDlg::OnSizing(UINT fwSide, LPRECT pRect)
{
	CDialogEx::OnSizing(fwSide, pRect);

	// TODO: �ڴ˴������Ϣ����������
}


void CGLdetectionDlg::OnSize(UINT nType, int cx, int cy)
{
	
// 	CWnd *pWnd;
// 
// 	pWnd = GetDlgItem(IDC_STATIC_SO2_T);//��ȡ�ؼ����
// 
// 	if (pWnd)//�ж��Ƿ�Ϊ�գ���Ϊ�Ի��򴴽�ʱ����ô˺���������ʱ�ؼ���δ����
// 
// 	{
// 		CRect rect;//��ȡ�ؼ��仯ǰ��С
// 		pWnd->GetWindowRect(&rect);
// 		ScreenToClient(&rect);//���ؼ���Сת��Ϊ�ڶԻ����е���������
// 							  //��cx/m_rect.Width()Ϊ�Ի����ں���ı仯����
// 		rect.left = rect.left*cx / m_rect.Width();/////�����ؼ���С
// 		rect.right = rect.right*cx / m_rect.Width();
// 		rect.top = rect.top*cy / m_rect.Height();
// 		rect.bottom = rect.bottom*cy / m_rect.Height();
// 		pWnd->MoveWindow(rect);//���ÿؼ���С
// 	}
// 	GetClientRect(&m_rect);//���仯��ĶԻ����С��Ϊ�ɴ�С
	if (nType == SIZE_RESTORED || nType == SIZE_MAXIMIZED) //
	{
		ReSize();

		
#ifdef FACE_TWO
		if (((CButton*)GetDlgItem(IDC_STATIC_NO)))
		{
			CRect rectTemp, rectText;
			((CButton*)GetDlgItem(IDC_STATIC_NO))->GetWindowRect(rectTemp);
			ScreenToClient(&rectTemp);
			((CButton*)GetDlgItem(IDC_STATIC_NO_T))->GetWindowRect(rectText);
			ScreenToClient(&rectText);
			((CButton*)GetDlgItem(IDC_STATIC_NO_T))->SetWindowPos(NULL
				, rectTemp.right
				, rectTemp.bottom - rectText.Height()
				, rectText.Width()
				, rectText.Height(), SWP_SHOWWINDOW);
		}
		if (((CButton*)GetDlgItem(IDC_STATIC_CO)))
		{
			CRect rectTemp, rectText;
			((CButton*)GetDlgItem(IDC_STATIC_CO))->GetWindowRect(rectTemp);
			ScreenToClient(&rectTemp);
			((CButton*)GetDlgItem(IDC_STATIC_CO_T))->GetWindowRect(rectText);
			ScreenToClient(&rectText);
			((CButton*)GetDlgItem(IDC_STATIC_CO_T))->SetWindowPos(NULL
				, rectTemp.right
				, rectTemp.bottom - rectText.Height()
				, rectText.Width()
				, rectText.Height(), SWP_SHOWWINDOW);
		}
		if (((CButton*)GetDlgItem(IDC_STATIC_NO2)))
		{
			CRect rectTemp, rectText;
			((CButton*)GetDlgItem(IDC_STATIC_NO2))->GetWindowRect(rectTemp);
			ScreenToClient(&rectTemp);
			((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->GetWindowRect(rectText);
			ScreenToClient(&rectText);
			((CButton*)GetDlgItem(IDC_STATIC_NO2_T))->SetWindowPos(NULL
				, rectTemp.right
				, rectTemp.bottom - rectText.Height()
				, rectText.Width()
				, rectText.Height(), SWP_SHOWWINDOW);
		}
		if (((CButton*)GetDlgItem(IDC_STATIC_O2)))
		{
			CRect rectTemp, rectText;
			((CButton*)GetDlgItem(IDC_STATIC_O2))->GetWindowRect(rectTemp);
			ScreenToClient(&rectTemp);
			((CButton*)GetDlgItem(IDC_STATIC_O2_T))->GetWindowRect(rectText);
			ScreenToClient(&rectText);
			((CButton*)GetDlgItem(IDC_STATIC_O2_T))->SetWindowPos(NULL
				, rectTemp.right
				, rectTemp.bottom - rectText.Height()
				, rectText.Width()
				, rectText.Height(), SWP_SHOWWINDOW);
		}
#endif
		Invalidate(TRUE);
	}
	CDialogEx::OnSize(nType, cx, cy);
}

/*ʵʱ��ʾ���ݵ�������*/
BOOL CGLdetectionDlg::ChangeListData(CStringArray const& strAry)
{
// 	m_listctrl_show.SetRedraw(FALSE);
// 	//��������
// 	
// 	int curItem = m_listctrl_show.GetItemCount();
// 	for (int index = 0; index < strAry.GetSize();++index)
// 	{
// 		m_listctrl_show.SetItemText(curItem+1, index+1, strAry.GetAt(index));
// 	}
// 	m_listctrl_show.SetRedraw(TRUE);
// 	m_listctrl_show.Invalidate();
// 	m_listctrl_show.UpdateWindow();
	if (strAry.GetSize() < 2)
	{
		return FALSE;
	}
	int index = _ttoi(strAry.GetAt(0));
	CString strTemp = strAry.GetAt(1);

	if (LISTSHOW_CO2 == index
		|| LISTSHOW_O2 == index)
	{
		strTemp.Format(_T("%.02f"),_ttoi(strTemp)/100.0);
	}
	CFont* font = nullptr;
	LOGFONT lf;
	int fontNum = 0;
	int fontSize = 0;
	CButton* pButton = nullptr,*pButtonT=nullptr,*pButtonG=nullptr;
	CRect buttonRect,buttonTRect,buttonGRect;
	
	if (controlID[index] != -1)
	{
		pButton = ((CButton*)GetDlgItem(controlID[index]));
		if (pButton)
		{
			pButton->SetWindowText(strTemp);
		}
#if 0
		pButton = ((CButton*)GetDlgItem(controlID[index]));
		pButtonT = ((CButton*)GetDlgItem(controlIDT[index]));
		pButtonG = ((CButton*)GetDlgItem(controlIDG[index]));

		pButton->GetWindowRect(&buttonRect);
		pButtonT->GetWindowRect(&buttonTRect);
		pButtonG->GetWindowRect(&buttonGRect);

		ScreenToClient(&buttonRect);
		ScreenToClient(&buttonTRect);
		ScreenToClient(&buttonGRect);

		CRect rect;
		CSize size(0, 0);
		CClientDC dc(pButton);
		CFont *pOldFont = dc.SelectObject(pButton->GetFont());

		if (::GetTextExtentPoint32((HDC)dc, strTemp, strTemp.GetLength(), &size))
		{
			rect.left = buttonGRect.left + (buttonGRect.Width() - size.cx - buttonTRect.Width())/2;
			rect.top = buttonRect.top;
			rect.right = rect.left + size.cx;
			rect.bottom = buttonRect.bottom;
			
			pButton->MoveWindow(rect);
		}
	
		dc.SelectObject(pOldFont);

		

		rect.left = rect.right;
		rect.top = buttonTRect.top;
		rect.right = rect.left + buttonTRect.Width();
		rect.bottom = buttonTRect.bottom;
		pButtonT->MoveWindow(rect);
#endif
	}

	
	return  TRUE;
}




HBRUSH CGLdetectionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{

	

	
//����2
#ifdef FACE_TWO
	HBRUSH hbr = CDialogEx::OnCtlColor(pDC, pWnd, nCtlColor);
	return hbr;
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_NO
		||pWnd->GetDlgCtrlID() == IDC_STATIC_NO2
		||pWnd->GetDlgCtrlID() == IDC_STATIC_CO
		||pWnd->GetDlgCtrlID() == IDC_STATIC_O2)//����Ǳ༭��
	{
	
		
	
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO2_T);
	}
	if (pWnd->GetDlgCtrlID() == IDC_STATIC_NO_T
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_NO2_T
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_CO_T
		|| pWnd->GetDlgCtrlID() == IDC_STATIC_O2_T)//����Ǳ༭��
	{
		  
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO2_T);
	}
	return hbr;
#else

	//CFont font;
	// TODO:  �ڴ˸��� DC ���κ�����
// 	if (pWnd->GetDlgCtrlID() == )
// 	{
// 		pDC->SetBkMode(TRANSPARENT); //������޸����屳��ģʽ
// 									 //pDC-> SetBkColor(m_bkcolor); //������޸ı�����ɫ
// 									 //hbr=CreateSolidBrush(m_bkcolor); //������޸Ļ�ˢ
// 		//pDC->SetTextColor(RGB(255, 0, 0)); //�㵱Ȼ�����޸�������ɫ
// 		
// 		HBRUSH b = CreateSolidBrush(RGB(255, 255, 255));
// 		return b;//��������
// 	}
	switch (pWnd->GetDlgCtrlID())
	{
	case IDC_STATIC_SHOW:
		//pDC->SetBkMode(TRANSPARENT);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICSO2);//
  //��ʾ��6������� static �ؼ�
	case IDC_STATIC_SO2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICSO2);
		
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICSO2);//
		break;

	case IDC_STATIC_NO:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO);//
		break;
	case IDC_STATIC_CO:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO);//
		break;
	case IDC_STATIC_NO2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO2);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO2);//
		break;
	case IDC_STATIC_O2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICO2);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICO2);//
		break;
	case IDC_STATIC_CO2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO2);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO2);//
		break;


	case IDC_STATIC_SO2_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICSO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICSO2_T);//
		break;
	case IDC_STATIC_NO_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO_T);//
		break;
	case IDC_STATIC_CO_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO_T);//
		break;
	case IDC_STATIC_NO2_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO2_T);//
		break;
	case IDC_STATIC_O2_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICO2);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICO2);//
		break;
	case IDC_STATIC_CO2_T:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO2_T);//
		break;

	case IDC_STATIC_Bit_SO2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICSO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICSO2_T);//
		break;
	case IDC_STATIC_Bit_NO:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO_T);//
		break;
	case IDC_STATIC_Bit_CO:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO_T);//
		break;
	case IDC_STATIC_Bit_NO2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICNO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICNO2_T);//
		break;
	case IDC_STATIC_Bit_O2:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICO2_T);//
		break;
	case IDC_STATIC_Bit_CO2: 
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(STATICCO2_T);
		return Change_skin::GetInstance(this)->GetBrush(brush_STATICCO2_T);//
		break;


	default:
		pDC->SetBkMode(TRANSPARENT);
		//hbr = CreateSolidBrush(RGB(255, 255, 255));

		return Change_skin::GetInstance(this)->GetWhiteBrush();//
		break;

	}
	
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
#endif
}


void CGLdetectionDlg::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return CDialogEx::OnDrawItem(nIDCtl, lpDrawItemStruct);	
}


BOOL CGLdetectionDlg::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::PreCreateWindow(cs);
}


void CGLdetectionDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_nWidth = GetSystemMetrics(SM_CXSCREEN);
	m_nHeight = GetSystemMetrics(SM_CYSCREEN);
	

	lpMMI->ptMinTrackSize.x = 1280;
	lpMMI->ptMinTrackSize.y = 1024;

	//lpMMI->ptMinTrackSize = point;
	//lpMMI->ptMaxTrackSize = point;
	CDialogEx::OnGetMinMaxInfo(lpMMI);
}



BOOL CGLdetectionDlg::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	
	return CDialogEx::OnEraseBkgnd(pDC);
}


void CGLdetectionDlg::OnNcCalcSize(BOOL bCalcValidRects, NCCALCSIZE_PARAMS* lpncsp)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
// 	CRect rect;
// 	GetWindowRect(&rect);
// 	ScreenToClient(&rect);
// 	lpncsp->rgrc[0].top += 60 - GetSystemMetrics(SM_CYCAPTION) - GetSystemMetrics(SM_CYSMCAPTION);
// 	lpncsp->rgrc[0].left += rect.left - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME);
// 	lpncsp->rgrc[0].right -= rect.right - GetSystemMetrics(SM_CXFRAME) - GetSystemMetrics(SM_CXSIZEFRAME);
// 	lpncsp->rgrc[0].bottom -= rect.bottom - GetSystemMetrics(SM_CYFRAME) - GetSystemMetrics(SM_CYSIZEFRAME);
	CDialogEx::OnNcCalcSize(bCalcValidRects, lpncsp);
}


void CGLdetectionDlg::InitFaceData(void)
{
	/*��ʼ�� ���ݰ�, 0���� ��ʾָ�� 1������ʾ ���ص�����*/
	m_strAryJTData.RemoveAll();
	m_strAryJTData.SetSize(2);

	/*��ʼ������ �ؼ�*/
	//1.��ʼ��COM�ڵ�COMBOX�ؼ�
	m_strBTL = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_BTL"));
	m_strSJW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_SJW"));
	m_strJYW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_JYW"));
	m_strTZW = CReadWriteConfig::ConfigGetString(_T("COM"), _T("COM_TZW"));
	int maxNum = CReadWriteConfig::ConfigGetInt(_T("COM"), _T("COM_MAX"));
	int curNum = CReadWriteConfig::ConfigGetInt(_T("COM"), _T("COM_CUR"));
	if (m_strBTL.IsEmpty())
	{
		m_strBTL = _T("4800");
	}
	if (m_strSJW.IsEmpty())
	{
		m_strSJW = _T("8");
	}
	if (m_strJYW.IsEmpty())
	{
		m_strJYW = _T("N");
	}
	if (m_strTZW.IsEmpty())
	{
		m_strTZW = _T("1");
	}
	if (maxNum <= 0)
	{
		maxNum = 10;
	}
	if (curNum <= 0)
	{
		curNum = 1;
	}
	CString strTemp;
	for (int index = 1; index <= maxNum; ++index)
	{
		strTemp.Format(_T("COM%d"), index);
		m_combox_com.AddString(strTemp);
	}
	m_combox_com.SetCurSel(curNum - 1);

	UpdateData(FALSE);
}

void CGLdetectionDlg::ReSize()
{
	
	float fsp[2];
	POINT Newp; //��ȡ���ڶԻ���Ĵ�С  
	CRect recta;
	GetClientRect(&recta);     //ȡ�ͻ�����С    
	Newp.x = recta.right - recta.left;
	Newp.y = recta.bottom - recta.top;
	fsp[0] = (float)Newp.x / old.x;
	fsp[1] = (float)Newp.y / old.y;
	CRect Rect;
	int woc;
	CPoint OldTLPoint, TLPoint; //���Ͻ�  
	CPoint OldBRPoint, BRPoint; //���½�  
	HWND  hwndChild = ::GetWindow(m_hWnd, GW_CHILD);  //�г����пؼ�    
	while (hwndChild)
	{
		woc = ::GetDlgCtrlID(hwndChild);//ȡ��ID  
		if (IDC_COMBO_COM == woc) //COMBOX ����
		{
			
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			int w = Rect.Width();
			int h = Rect.Height();
			Rect.SetRect(TLPoint.x, TLPoint.y, TLPoint.x + w, TLPoint.y + h);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		if (IDC_STATIC_LOG == woc) //LOGO 
		{
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			int w = Rect.Width();
			int h = Rect.Height();
			Rect.SetRect(TLPoint.x, TLPoint.y, TLPoint.x + w*fsp[0], TLPoint.y + h*fsp[1]);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		if (IDC_STATIC_LEFT_SPLIT == woc) //�ָ���
		{
			
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			int w = Rect.Width();
			int h = Rect.Height();
			Rect.SetRect(TLPoint.x, TLPoint.y, TLPoint.x + w, TLPoint.y + h);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}
		if (IDC_STATIC_NO == woc 
			||IDC_STATIC_CO == woc
			||IDC_STATIC_NO2 == woc
			||IDC_STATIC_O2 == woc
			)
		{
			GetDlgItem(woc)->GetWindowRect(Rect);
			ScreenToClient(Rect);
			OldTLPoint = Rect.TopLeft();
			TLPoint.x = long(OldTLPoint.x*fsp[0]);
			TLPoint.y = long(OldTLPoint.y*fsp[1]);
			int w = Rect.Width();
			int h = Rect.Height();
			Rect.SetRect(TLPoint.x, TLPoint.y, TLPoint.x + w, TLPoint.y + h);
			GetDlgItem(woc)->MoveWindow(Rect, TRUE);
			hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
			continue;
		}

		GetDlgItem(woc)->GetWindowRect(Rect);
		ScreenToClient(Rect);
		OldTLPoint = Rect.TopLeft();
		TLPoint.x = long(OldTLPoint.x*fsp[0]);
		TLPoint.y = long(OldTLPoint.y*fsp[1]);
		OldBRPoint = Rect.BottomRight();
		BRPoint.x = long(OldBRPoint.x *fsp[0]);
		BRPoint.y = long(OldBRPoint.y *fsp[1]);
		Rect.SetRect(TLPoint, BRPoint);
		GetDlgItem(woc)->MoveWindow(Rect, TRUE);
		hwndChild = ::GetWindow(hwndChild, GW_HWNDNEXT);
	}
	old = Newp;
	



}

LRESULT CGLdetectionDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: �ڴ����ר�ô����/����û���

	return CDialogEx::WindowProc(message, wParam, lParam);
}


afx_msg LRESULT CGLdetectionDlg::OnShowdata(WPARAM wParam, LPARAM lParam)
{
	ChangeListData(m_strAryJTData); //��ʾ
	return 0;
}


void CGLdetectionDlg::OnClose()
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_stopFlag = false;
	m_stopSearch = false;
	CDialogEx::OnClose();
}


//��ʾ ���� ��ں���
bool CGLdetectionDlg::ShowListData(CString const& strData)
{
	//��ȡ ��������
	CStringArray strAry;
	CString strTemp = strData;
	int begin = 0, end = 0, result = 0;
	int size = getFun(strTemp, gStrFlag); //���ݴ� ����
	if (size <= 0)
		return false;
	const int strLength = 34, maxSize = 5;
	CString resTemp;
	int beg = -1;
	for (int index = 0; index < size && index < maxSize && strTemp.GetLength() >= strLength; ++index)
	{
		beg = strTemp.Find(gStrFlag);
		if (beg != -1)
		{
			resTemp = strTemp.Mid(beg, strLength);
			if (resTemp.GetLength() == strLength)
			{
				strAry.Add(resTemp);
			}
			else
			{
				break;
			}
			if (strAry.GetSize() >= maxSize)
			{
				break;
			}
			strTemp = StrStrI(strTemp, gStrFlag) + strLength;

		}
		else
		{
			break;
		}
	}

	//û����Ч����
	if (strAry.GetSize() < 1)
	{
		return false;
	}
	CStringArray strShowAry;
	int begIndex = 0; //���ݿ�ʼλ��
	for (int index = 0; index < strAry.GetSize(); ++index)
	{
		strTemp = strAry.GetAt(index); //ȡ ��һ������
		if (strTemp.GetLength() != strLength)
			continue;

		unsigned char buf[17] = { 0 };
		while (begIndex < strLength)
		{
			//��Ч��Χ
			{
				sscanf_s(CpublicFun::UnicodeToAsc(strTemp.Mid(begIndex, 2)), "%x", &begin);

				buf[begIndex / 2] = begin;

				//result = ((begin << 8) | end)&(~0x8000);
				//strTemp.Format(_T("%d"), result);
			}
			begIndex += 2;
		}

		
		if (0 == get_crc16(buf, 17))
		{
			for (int index = 3; index < 15; index += 2)
			{
				result = ((buf[index] << 8) | buf[index + 1])&(~0x8000);
				strTemp.Format(_T("%d"), result);
				strShowAry.Add(strTemp);
			}
			break;
		}

	}

	int indexTemp = -1;
	for (int index = 0; strShowAry.GetSize() && controlID[index] != -1; ++index)
	{
		indexTemp = idToControlId[index];
		if (-1 == indexTemp)
		{
			continue;
		}
		strTemp = strShowAry.GetAt(index);
		if (LISTSHOW_CO2 == indexTemp
			|| LISTSHOW_O2 == indexTemp)
		{
			strTemp.Format(_T("%.02f"), _ttoi(strTemp) / 100.0);
		}

		if (controlID[indexTemp] != -1)
		{
			((CButton*)GetDlgItem(controlID[indexTemp]))->SetWindowText(strTemp);
		}
	}
	return true;
}
