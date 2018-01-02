
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持




#include <comdef.h>//初始化一下com口
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
* 界面2
* 启动宏
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

// _TEXT("二氧化硫"), ppm
// _TEXT("一氧化氮"),
// _TEXT("一氧化碳"),
// _TEXT("氢气"),
// _TEXT("氧气"), %
// _TEXT("二氧化碳"), %

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
/*控件的颜色*/
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