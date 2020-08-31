#include "stdafx.h"
#include "Path.h"

namespace Hootina
{

CPath::CPath(void)
{
}

CPath::~CPath(void)
{
}

// ��ȡӦ�ó���ִ��·��
tstring CPath::GetAppPath()
{
	TCHAR szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL, szPath, MAX_PATH);

	tstring strPath(szPath);
	for (int nPos = (int)strPath.size()-1; nPos >= 0; --nPos)
	{
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
			return strPath.substr(0, nPos+1);
	}
	return strPath;
}

std::string CPath::GetAppPathAscii()
{
	char szPath[MAX_PATH] = {0};
	::GetModuleFileNameA(NULL, szPath, MAX_PATH);

	std::string strPath(szPath);
	for (int nPos = (int)strPath.size()-1; nPos >= 0; --nPos)
	{
		char cChar = strPath[nPos];
		if ('\\' == cChar || '/' == cChar)
			return strPath.substr(0, nPos+1);
	}
	return strPath;
}

// ��ȡӦ�ó���ǰĿ¼
tstring CPath::GetCurDir()
{
	TCHAR szCurDir[MAX_PATH] = {0};
	::GetCurrentDirectory(MAX_PATH, szCurDir);

	DWORD dwLen = _tcslen(szCurDir);
	if (dwLen <= 0)
		return _T("");

	TCHAR cLastChar = szCurDir[dwLen - 1];
	if (cLastChar != _T('\\') && cLastChar != _T('/'))
		_tcscat(szCurDir, _T("\\"));

	return szCurDir;
}

// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
tstring CPath::GetTempPath()
{
	TCHAR szTempPath[MAX_PATH] = {0};
	::GetTempPath(MAX_PATH, szTempPath);

	DWORD dwLen = _tcslen(szTempPath);
	if (dwLen <= 0)
		return _T("");

	TCHAR cLastChar = szTempPath[dwLen - 1];
	if (cLastChar != _T('\\') && cLastChar != _T('/'))
		_tcscat(szTempPath, _T("\\"));

	return szTempPath;
}

// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
tstring CPath::GetTempFileName(LPCTSTR lpszFileName)
{
	return GetRandomFileName(GetTempPath().c_str(), lpszFileName);
}

// ��ȡ����ļ���(ȫ·��)
tstring CPath::GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName)
{
	tstring strPath, strFileName, strExtFileName, strFullPath;
	TCHAR szBuf[MAX_PATH] = {0};

	if (!IsDirectoryExist(lpszPath))
		strPath = GetAppPath();
	else
		strPath = lpszPath;

	strFileName = GetFileNameWithoutExtension(lpszFileName);
	strExtFileName = GetExtension(lpszFileName);

	for (int i = 2; i < 10000; i++)
	{
		if (strExtFileName.empty())
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d"), i);
			strFullPath += szBuf;
		}
		else
		{
			strFullPath = strPath;
			strFullPath += strFileName;
			wsprintf(szBuf, _T("%d."), i);
			strFullPath += szBuf;
			strFullPath += strExtFileName;
		}
		
		if (!IsFileExist(strFullPath.c_str()))
			return strFullPath;
	}

	return _T("");
}

// ���ָ��·���Ƿ�Ŀ¼
BOOL CPath::IsDirectory(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);
	return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
}

// ���ָ���ļ��Ƿ����
BOOL CPath::IsFileExist(LPCTSTR lpszFileName)
{
	if (NULL == lpszFileName || NULL ==*lpszFileName)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszFileName);
	return (((dwAttr != 0xFFFFFFFF) && (!(dwAttr & FILE_ATTRIBUTE_DIRECTORY))) ? TRUE : FALSE);
}

// ���ָ��Ŀ¼�Ƿ����
BOOL CPath::IsDirectoryExist(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return FALSE;

	DWORD dwAttr = ::GetFileAttributes(lpszPath);
	return (((dwAttr != 0xFFFFFFFF) && (dwAttr & FILE_ATTRIBUTE_DIRECTORY)) ? TRUE : FALSE);
}

BOOL CPath::CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes)
{
	TCHAR cPath[MAX_PATH] = {0};
	TCHAR cTmpPath[MAX_PATH] = {0};
	TCHAR* lpPos = NULL;
	TCHAR cTmp = _T('\0');

	if (NULL == lpPathName || NULL ==*lpPathName)
		return FALSE;

	_tcsncpy(cPath, lpPathName, MAX_PATH);

	for (int i = 0; i < (int)_tcslen(cPath); i++)
	{
		if (_T('\\') == cPath[i])
			cPath[i] = _T('/');
	}

	lpPos = _tcschr(cPath, _T('/'));
	while (lpPos != NULL)
	{
		if (lpPos == cPath)
		{
			lpPos++;
		}
		else
		{
			cTmp =*lpPos;
			*lpPos = _T('\0');
			_tcsncpy(cTmpPath, cPath, MAX_PATH);
			::CreateDirectory(cTmpPath, lpSecurityAttributes);
			*lpPos = cTmp;
			lpPos++;
		}
		lpPos = _tcschr(lpPos, _T('/'));
	}

	return TRUE;
}

// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
tstring CPath::GetPathRoot(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::iterator iter;
	for (iter = strPath.begin(); iter < strPath.end(); iter++)
	{
		if (_T('\\') ==*iter)
			*iter = _T('/');
	}

	tstring::size_type nPos = strPath.find(_T('/'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos+1);

	return strPath;
}

// ����ָ��·���ַ�����Ŀ¼��Ϣ
tstring CPath::GetDirectoryName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);
	for (int nPos = (int)strPath.size()-1; nPos >= 0; --nPos)
	{
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
			return strPath.substr(0, nPos+1);
	}
	return strPath;
}

// ����ָ��·���ַ������ļ�������չ��
tstring CPath::GetFileName(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);
	for (int nPos = (int)strPath.size()-1; nPos >= 0; --nPos)
	{
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
			return strPath.substr(nPos+1);
	}
	return strPath;
}

// ���ز�������չ����·���ַ������ļ���
tstring CPath::GetFileNameWithoutExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);
	for (int nPos = (int)strPath.size()-1; nPos >= 0; --nPos)
	{
		TCHAR cChar = strPath[nPos];
		if (_T('\\') == cChar || _T('/') == cChar)
		{
			strPath = strPath.substr(nPos+1);
			break;
		}
	}

	int nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
		strPath = strPath.substr(0, nPos);

	return strPath;
}

// ����ָ����·���ַ�������չ��
tstring CPath::GetExtension(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	tstring::size_type nPos = strPath.rfind(_T('.'));
	if (nPos != tstring::npos)
		return strPath.substr(nPos+1);
	else
		return _T("");
}

// ����ָ�������·����ȡ����·��
tstring CPath::GetFullPath(LPCTSTR lpszPath)
{
	if (NULL == lpszPath || NULL ==*lpszPath)
		return _T("");

	tstring strPath(lpszPath);

	TCHAR cFirstChar = strPath.at(0);
	TCHAR cSecondChar = strPath.at(1);

	if (cFirstChar == _T('\\') || cFirstChar == _T('/'))
	{
		tstring strCurDir = GetAppPath();
		tstring strRootPath = GetPathRoot(strCurDir.c_str());
		return strRootPath + strPath;
	}
	else if (::IsCharAlpha(cFirstChar) && cSecondChar == _T(':'))
	{
		return strPath;
	}
	else
	{
		tstring strCurDir = GetAppPath();
		return strCurDir + strPath;
	}
}

long CPath::GetFileSize(LPCTSTR lpszPath)
{
	if(lpszPath == NULL)
		return 0;

	WIN32_FIND_DATA fd = {0};

	HANDLE hFindFile = ::FindFirstFile(lpszPath, &fd);
	if(hFindFile == INVALID_HANDLE_VALUE)
		return 0;
	
	//TODO: �����Ǵ���4G���ϵ��ļ�
	long nFileSize = (long)fd.nFileSizeLow;
	::FindClose(hFindFile);

	return nFileSize;
}

}