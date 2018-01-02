#pragma once



class CReadWriteConfig
{
public:
	~CReadWriteConfig(void);
private:
	CReadWriteConfig(void);
	static CReadWriteConfig singleton;
public:
	static BOOL			ConfigInit();
	static CString		ReturnOCXPath();
	static CString		ReturnCALLPath();
	static CString		ConfigGetString(LPCTSTR lpAppname, LPCTSTR lpKeyname);
	static int			ConfigGetInt(LPCTSTR lpAppname, LPCTSTR lpKeyname);
	static void			ConfigSetValue(LPCTSTR lpAppname, LPCTSTR lpKeyname, LPCTSTR lpValue);
private:
	CString strPath;
};

