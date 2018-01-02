
// GLdetection.cpp : 定义应用程序的类行为。
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


// CGLdetectionApp 构造

CGLdetectionApp::CGLdetectionApp()
{
	// 支持重新启动管理器
	m_dwRestartManagerSupportFlags = AFX_RESTART_MANAGER_SUPPORT_RESTART;

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CGLdetectionApp 对象

CGLdetectionApp theApp;

// 创建Dump文件  
//   
void CreateDumpFile(LPCWSTR lpstrDumpFilePathName, EXCEPTION_POINTERS *pException)
{
	// 创建Dump文件  
	//  
	HANDLE hDumpFile = CreateFile(lpstrDumpFilePathName, GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);

	// Dump信息  
	//  
	MINIDUMP_EXCEPTION_INFORMATION dumpInfo;
	dumpInfo.ExceptionPointers = pException;
	dumpInfo.ThreadId = GetCurrentThreadId();
	dumpInfo.ClientPointers = TRUE;

	// 写入Dump文件内容  
	//  
	MiniDumpWriteDump(GetCurrentProcess(), GetCurrentProcessId(), hDumpFile, MiniDumpNormal, &dumpInfo, NULL, NULL);

	CloseHandle(hDumpFile);
}
// 处理Unhandled Exception的回调函数  
//  
LONG ApplicationCrashHandler(EXCEPTION_POINTERS *pException)
{
	// 这里弹出一个错误对话框并退出程序  
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
	//MessageBox(NULL, L"系统错误,麻烦请联系技术人员!", L"错误提示", MB_OK);

	
	CString strPath = CLogRecord::GetAppPath() + _T("\\GLdetection.exe");
	//重启

	//重启程序  
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

// CGLdetectionApp 初始化
//gdi+用到的两个变量   装载gdi+
GdiplusStartupInput m_gdiplusStartupInput;
ULONG_PTR m_pGdiToken;
BOOL CGLdetectionApp::InitInstance()
{

	
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。  否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();
	
	// 创建 shell 管理器，以防对话框包含
	// 任何 shell 树视图控件或 shell 列表视图控件。
	CShellManager *pShellManager = new CShellManager;

	// 激活“Windows Native”视觉管理器，以便在 MFC 控件中启用主题
	CMFCVisualManager::SetDefaultManager(RUNTIME_CLASS(CMFCVisualManagerWindows));

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);


	SetUnhandledExceptionFilter((LPTOP_LEVEL_EXCEPTION_FILTER)ApplicationCrashHandler);
	
	
	/*1.软件单例{82F6BEC0-7E87-490C-AB77-C555296DE976}*/
	CreateEvent(NULL, FALSE, TRUE, _T("{82F6BEC0-7E87-490C-AB77-C555296DE976}"));
	if (ERROR_ALREADY_EXISTS == GetLastError())
	{
		
		MessageBox(NULL, _T("软件已启动,重复启动!"), _T("提示"), MB_OK | MB_TOPMOST);
		return 0;
	}
	

	if (!AfxOleInit())
	{
		MessageBox(NULL, _T("ole初始化失败!"), _T("提示"), MB_OK | MB_TOPMOST);
		return 0;
	}

	/*2.软件初始化*/
	if (!CReadWriteConfig::ConfigInit())
	{
		MessageBox(NULL, _T("配置文件初始化失败,缺失baseConfig.ini文件!"), _T("提示"), MB_OK | MB_TOPMOST);
		return FALSE;
	}
	if (!CLogRecord::InitLogRecord())
	{
		MessageBox(NULL, _T("日志文件初始化失败,缺失logfiles文件夹!"), _T("提示"), MB_OK | MB_TOPMOST);
		return FALSE;
	}
	/*每次启动都注册一次控件*/
//	if (!CpublicFun::RegistOcx(_T("MSCOMM32.OCX")))
//	{
//		MessageBox(NULL, _T("mscomm32.ocx控件注册失败,请手动注册!"), _T("提示"), MB_OK | MB_TOPMOST);
//		return FALSE;
//	}
#ifdef CSKIN
	CSkinManager::GetInstance()->SetSkinPath(CLogRecord::GetAppPath() + _T("\\Skins\\Default\\"));
#endif
	CLogRecord::WriteRecordToFile(_T("-----------------------软件正常启动!-----------------------"));
	
	CGLdetectionDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用
		//  “确定”来关闭对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用
		//  “取消”来关闭对话框的代码
	}
	else if (nResponse == -1)
	{
		TRACE(traceAppMsg, 0, "警告: 对话框创建失败，应用程序将意外终止。\n");
		TRACE(traceAppMsg, 0, "警告: 如果您在对话框上使用 MFC 控件，则无法 #define _AFX_NO_MFC_CONTROLS_IN_DIALOGS。\n");
	}

	// 删除上面创建的 shell 管理器。
	if (pShellManager != NULL)
	{
		delete pShellManager;
	}

#ifndef _AFXDLL
	ControlBarCleanUp();
#endif

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}



int CGLdetectionApp::ExitInstance()
{
	// TODO: 在此添加专用代码和/或调用基类
	CLogRecord::WriteRecordToFile(_T("-----------------------软件正常退出!-----------------------"));
	GdiplusStartup(&m_pGdiToken, &m_gdiplusStartupInput, NULL);
	_CrtDumpMemoryLeaks();
	return CWinApp::ExitInstance();
}
