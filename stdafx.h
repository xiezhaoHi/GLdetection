
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��




#include <comdef.h>//��ʼ��һ��com��
// #ifndef ULONG_PTR
// #define ULONG_PTR unsigned long*
#include "GdiPlus.h"
using namespace Gdiplus;

#pragma comment(lib,"gdiplus.lib")

#include <tchar.h>
#include <vector>
#include <list>
#include <string>
#include <memory>
#include <sstream>

using namespace std;


/*
* ����2
* ������
*/
#define  FACE_TWO

#include "skin/SkinManager.h"

#define _CRTDBG_MAP_ALLOC
#include "stdlib.h"
#include "crtdbg.h"

#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif

// _TEXT("��������"), ppm
// _TEXT("һ������"),
// _TEXT("һ����̼"),
// _TEXT("����"),
// _TEXT("����"), %
// _TEXT("������̼"), %

enum LISTSHOW
{
	LISTSHOW_SO2 = 0,
	LISTSHOW_NO,
	LISTSHOW_CO,
	LISTSHOW_NO2,
	LISTSHOW_O2,
	LISTSHOW_CO2,
	LISTSHOW_MAX

};


//#define SHOW_MOUDLE
/*�ؼ�����ɫ*/
#ifdef SHOW_MOUDLE
#define  STATICSO2 RGB(0xf1, 0xb7, 0x06)	
#define  STATICNO RGB(0x20, 0x97, 0xf1)
#define  STATICCO RGB(0xcc, 0xdc, 0x38)
#define  STATICNO2 RGB(0x3e, 0x51, 0xb3)
#define  STATICO2 RGB(0xfe, 0x90, 0x00)
#define  STATICCO2 RGB(0xe8, 0x1e, 0x62)

#define  STATICSO2_T RGB(0xf1, 0xb7, 0x06)
#define  STATICNO_T RGB(0x20, 0x97, 0xf1)
#define  STATICCO_T RGB(0xcc, 0xdc, 0x38)
#define  STATICNO2_T RGB(0x3e, 0x51, 0xb3)
#define  STATICO2_T RGB(0xf1, 0x90, 0x00)
#define  STATICCO2_T RGB(0xe8, 0x1e, 0x62)

#endif
enum Brush_enum
{
	brush_STATICSO2,
	brush_STATICNO,
	brush_STATICCO,
	brush_STATICNO2,
	brush_STATICO2,
	brush_STATICCO2,
	brush_STATICSO2_T,
	brush_STATICNO_T,
	brush_STATICCO_T,
	brush_STATICNO2_T,
	brush_STATICO2_T,
	brush_STATICCO2_T,
	
	brush_max
};
#define CSKIN