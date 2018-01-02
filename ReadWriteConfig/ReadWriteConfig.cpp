#include "stdafx.h"
#include "ReadWriteConfig.h"

CReadWriteConfig CReadWriteConfig::singleton;

CReadWriteConfig::CReadWriteConfig(void)
{
}

CReadWriteConfig::~CReadWriteConfig(void)
{
}

BOOL CReadWriteConfig::ConfigInit()
{
	try{
		
		singleton.strPath = ReturnCALLPath();
		singleton.strPath += _T("\\config\\baseConfig.ini");
		return TRUE;
	}
	catch(...){}
	return FALSE;
}

CString CReadWriteConfig::ConfigGetString(LPCTSTR lpAppname, LPCTSTR lpKeyname)
{
	try{
		TCHAR buf[200] = _T("\0");
		::GetPrivateProfileString(lpAppname, lpKeyname, _T(""), buf, 200, singleton.strPath);
		return buf;
	}
	catch(...){}
	return _T("");
}

int CReadWriteConfig::ConfigGetInt(LPCTSTR lpAppname, LPCTSTR lpKeyname)
{
	try{
		return ::GetPrivateProfileInt(lpAppname, lpKeyname, 0, singleton.strPath);
	}
	catch(...){}
	return 0;
}

void CReadWriteConfig::ConfigSetValue( LPCTSTR lpAppname, LPCTSTR lpKeyname, LPCTSTR lpValue )
{
	try{
		::WritePrivateProfileString(lpAppname, lpKeyname, lpValue, singleton.strPath);
	}
	catch(...){}
}

CString    CReadWriteConfig::ReturnOCXPath()  
{   
	CString    sPath;   
	GetModuleFileName(GetModuleHandle(_T("Activex.ocx")),sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	return    sPath;   
}

CString    CReadWriteConfig::ReturnCALLPath()  
{   
	CString    sPath;   
	GetModuleFileName(NULL,sPath.GetBufferSetLength(MAX_PATH+1),MAX_PATH);   
	sPath.ReleaseBuffer();   
	int    nPos;   
	nPos=sPath.ReverseFind('\\');   
	sPath=sPath.Left(nPos);   
	return    sPath;   
}

