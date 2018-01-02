
// GLdetection.cpp : ����Ӧ�ó��������Ϊ��
//

#include "stdafx.h"
#include "GLdetection.h"
#include "GLdetectionDlg.h"
#include "ReadWriteConfig\ReadWriteConfig.h"
#include "logrecord\LogRecord.h"
#include "public\publicFun.h"
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#include <windows.h>  
#include <imagehlp.h>  
#include <stdlib.h>  
#pragma comment(lib, "dbghelp.lib")  

// CGLdetectionApp

BEGIN_MESSAGE_MAP(CGLdetectionApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CGLdetectionApp ����

CGLdetectionApp::CGLdetectionApp()
{
	// ֧����������������
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: �ڴ˴���ӹ�����룬
	// ��������Ҫ�ĳ�ʼ�������� InitInstance ��
}


// Ψһ��һ�� CGLdetectionApp ����

CGLdetectionApp theApp;

// ����Dump�ļ�  
//   
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// ����Dump�ļ�  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump��Ϣ  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// д��Dump�ļ�����  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}
// ����Unhandled Exception�Ļص�����  
//  
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	// ���ﵯ��һ������Ի����˳�����  
	//  
	//FatalAppExit(-1, _T("*** Unhandled Exception! ***"));

	CString strFile;
	TCHAR buff[1024] = { 0 };
	GetModuleFileName(NULL, buff, 1024);
	strFile = buff;
	int index = strFile.ReverseFind('\\');
	strFile = strFile.Left(index);
	CTime time = CTime::GetCurrentTime();
	strFile.Format(_T("%s\\dmp\\%s.dmp"), strFile, time.Format(_T("%Y%m%d%H%M%S")));
	CreateDumpFile(strFile, pException);
	//MessageBox(NULL, L"ϵͳ����,�鷳����ϵ������Ա!", L"������ʾ", MB_OK);

	
	CString strPath = CLogRecord::GetAppPath() + _T("\\GLdetection.exe");
	//����

	//��������  
	STARTUPINFO StartInfo;
	PROCESS_INFORMATION procStruct;
	memset(&StartInfo, 0, sizeof(STARTUPINFO));
	StartInfo.cb = sizeof(STARTUPINFO);
	::CreateProcess(
		(LPCTSTR)strPath,
		NULL,
		NULL,
		NULL,
		FALSE,
		NORMAL_PRIORITY_CLASS,
		NULL,
		NULL,
		&StartInfo,
		&procStruct);

	return EXCEPTION_EXECUTE_HANDLER;
}

// CGLdetectionApp ��ʼ��
//gdi+�õ�����������   װ��gdi+
GdiplusStartupInput m_gdiplusStartupInput;
ULONG_PTR m_pGdiToken;
BOOL CGLdetectionApp::InitInstance()
{

	
	// ���һ�������� Windows XP �ϵ�Ӧ�ó����嵥ָ��Ҫ
	// ʹ�� ComCtl32.dll �汾 6 ����߰汾�����ÿ��ӻ���ʽ��
	//����Ҫ InitCommonControlsEx()��  ���򣬽��޷��������ڡ�
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// ��������Ϊ��������Ҫ��Ӧ�ó�����ʹ�õ�
	// �����ؼ��ࡣ
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	
	// ���� shell ���������Է��Ի������
	// �κ� shell ����ͼ�ؼ��� shell �б���ͼ�ؼ���
	CShellManager *pShellManager = new CShellManager;

	// ���Windows Native���Ӿ����������Ա��� MFC �ؼ�����������
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// ��׼��ʼ��
	// ���δʹ����Щ���ܲ�ϣ����С
	// ���տ�ִ���ļ��Ĵ�С����Ӧ�Ƴ�����
	// ����Ҫ���ض���ʼ������
	// �������ڴ洢���õ�ע�����
	// TODO: Ӧ�ʵ��޸ĸ��ַ�����
	// �����޸�Ϊ��˾����֯��
	SetRegistryKey(_T("Ӧ�ó��������ɵı���Ӧ�ó���"));

	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);


	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	
	
	/*1.�������{82F6BEC0-7E87-490C-AB77-C555296DE976}*/
	CreateEvent(NULL, FALSE, TRUE, _T("{82F6BEC0-7E87-490C-AB77-C555296DE976}"));
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		
		MessageBox(NULL, _T("���������,�ظ�����!"), _T("��ʾ"), MB_OK | MB_TOPMOST);
		return 0;
	}
	

	if (!AfxOleInit())
	{
		MessageBox(NULL, _T("ole��ʼ��ʧ��!"), _T("��ʾ"), MB_OK | MB_TOPMOST);
		return 0;
	}

	/*2.�����ʼ��*/
	if (!CReadWriteConfig::ConfigInit())
	{
		MessageBox(NULL, _T("�����ļ���ʼ��ʧ��,ȱʧbaseConfig.ini�ļ�!"), _T("��ʾ"), MB_OK | MB_TOPMOST);
		return FALSE;
	}
	if (!CLogRecord::InitLogRecord())
	{
		MessageBox(NULL, _T("��־�ļ���ʼ��ʧ��,ȱʧlogfiles�ļ���!"), _T("��ʾ"), MB_OK | MB_TOPMOST);
		return FALSE;
	}
	/*ÿ��������ע��һ�οؼ�*/
//	if (!CpublicFun::RegistOcx(_T("MSCOMM32.OCX")))
//	{
//		MessageBox(NULL, _T("mscomm32.ocx�ؼ�ע��ʧ��,���ֶ�ע��!"), _T("��ʾ"), MB_OK | MB_TOPMOST);
//		return FALSE;
//	}
#ifdef CSKIN
	CSkinManager::GetInstance()->SetSkinPath(CLogRecord::GetAppPath() + _T("\\Skins\\Default\\"));
#endif
	CLogRecord::WriteRecordToFile(_T("-----------------------�����������!-----------------------"));
	
	CGLdetectionDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȷ�������رնԻ���Ĵ���
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: �ڴ˷��ô����ʱ��
		//  ��ȡ�������رնԻ���Ĵ���
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "����: �Ի��򴴽�ʧ�ܣ�Ӧ�ó���������ֹ��\n");
		TRACE(traceAppMsg, 0, "����: ������ڶԻ�����ʹ�� MFC �ؼ������޷� #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS��\n");
	}

	// ɾ�����洴���� shell ��������
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// ���ڶԻ����ѹرգ����Խ����� FALSE �Ա��˳�Ӧ�ó���
	//  ����������Ӧ�ó������Ϣ�á�
	return FALSE;
}



int CGLdetectionApp::ExitInstance()
{
	// TODO: �ڴ����ר�ô����/����û���
	CLogRecord::WriteRecordToFile(_T("-----------------------��������˳�!-----------------------"));
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	_CrtDumpMemoryLeaks();
	return CWinApp::ExitInstance();
}
