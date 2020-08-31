#pragma once

#include <string>

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

namespace Hootina
{

class CPath
{
public:
	CPath(void);
	~CPath(void);

	static tstring GetAppPath();												// ��ȡӦ�ó���ִ��·��(exe·��)
	static std::string GetAppPathAscii();										// ��ȡӦ�ó���ִ��·��
	static tstring GetCurDir();													// ��ȡӦ�ó���ǰĿ¼(main����Ŀ¼)
	static tstring GetTempPath();												// ��ȡ��ǰϵͳ����ʱ�ļ��е�·��
	static tstring GetTempFileName(LPCTSTR lpszFileName);						// ��ȡ��ǰϵͳ����ʱ�ļ��е�·���µ�Ψһ��������ʱ�ļ���(ȫ·��)
	static tstring GetRandomFileName(LPCTSTR lpszPath, LPCTSTR lpszFileName);	// ��ȡ����ļ���(ȫ·��)

	static BOOL IsDirectory(LPCTSTR lpszPath);									// ���ָ��·���Ƿ�Ŀ¼
	static BOOL IsFileExist(LPCTSTR lpszFileName);								// ���ָ���ļ��Ƿ����
	static BOOL IsDirectoryExist(LPCTSTR lpszPath);								// ���ָ��Ŀ¼�Ƿ����
	static BOOL CreateDirectory(LPCTSTR lpPathName, LPSECURITY_ATTRIBUTES lpSecurityAttributes=NULL);

	static tstring GetPathRoot(LPCTSTR lpszPath);								// ��ȡָ��·���ĸ�Ŀ¼��Ϣ
	static tstring GetDirectoryName(LPCTSTR lpszPath);							// ����ָ��·���ַ�����Ŀ¼��Ϣ
	static tstring GetFileName(LPCTSTR lpszPath);								// ����ָ��·���ַ������ļ�������չ��
	static tstring GetFileNameWithoutExtension(LPCTSTR lpszPath);				// ���ز�������չ����·���ַ������ļ���
	static tstring GetExtension(LPCTSTR lpszPath);								// ����ָ����·���ַ�������չ��
	static tstring GetFullPath(LPCTSTR lpszPath);								// ����ָ�������·����ȡ����·��
	static long    GetFileSize(LPCTSTR lpszPath);								// ��ȡ�ļ���С
};

}