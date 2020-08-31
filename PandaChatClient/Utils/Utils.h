#pragma once

#include <vector>
#include <string>
#include <time.h>
#include <atlstr.h>

#if defined(UNICODE) || defined(_UNICODE)
	typedef std::wstring tstring;
#else
	typedef std::string tstring;
#endif

#define DEL(p) \
	if(p != NULL) \
	{			  \
		delete p; \
		p = NULL; \
	}

#define DEL_ARR(p) \
	if(p != NULL) \
	{			  \
		delete[] p; \
		p = NULL; \
	}




BOOL ToHexStr(const CHAR* lpStr, int nSrcLen, CHAR* lpHex, int nDestLen);
BOOL StrToHex(const CHAR* lpStr, CHAR* lpHex, int nLen);

u_short Swap16(u_short nValue);		// 16λ�������������ֽ����������ֽ���ת��
u_long Swap32(u_long nValue);		// 32λ�������������ֽ����������ֽ���ת��

COLORREF HexStrToRGB(LPCTSTR lpszStr);
void RGBToHexStr(COLORREF color, TCHAR* lpBuf, int nLen);

time_t DateToUTCSeconds(long nYear, long nMonth, long nDay);				//������ת����UTC����
void FormatTime(time_t lTime, LPCTSTR lpFmt, TCHAR* lpBuf, int nLen);
BOOL IsToday(time_t lTime);

BOOL IsToday(SYSTEMTIME* lpTime);
BOOL GetFileLastWriteTime(LPCTSTR lpszFileName, SYSTEMTIME* lpSysTime);	// ��ȡ�ļ�����޸�ʱ��

BOOL File_ReadAll(const TCHAR* lpszFileName, CHAR**lpData, LONG*lSize);	// ��ȡ�����ļ�

BOOL my_isalnum(unsigned char cChar);		// ���ָ���ַ��Ƿ�����ĸ(A-Z��a-z)������(0-9)
BOOL my_isalnum(wchar_t cChar);				// ���ָ���ַ��Ƿ�����ĸ(A-Z��a-z)������(0-9)

BOOL EnumSysFont(std::vector<tstring>* arrSysFont);	// ö��ϵͳ����
BOOL FlashWindowEx(HWND hWnd, int nCount);				// ��˸���ڱ�����
BOOL GetTrayWndRect(RECT* lpRect);						// ��ȡϵͳ����������

BOOL IsDigit(const WCHAR* lpStr);
void Replace(std::string& strText, const CHAR* lpOldStr, const CHAR* lpNewStr);
void Replace(std::wstring& strText, const WCHAR* lpOldStr, const WCHAR* lpNewStr);
WCHAR ToLower(WCHAR c);
void ToLower(WCHAR* lpText);
void ToLower(std::wstring& strText);
WCHAR ToUpper(WCHAR c);
void ToUpper(WCHAR* lpText);
void ToUpper(std::wstring& strText);
void EncodeHtmlSpecialChars(std::wstring& strText);		// ����Html�����ַ�
void DecodeHtmlSpecialChars(std::wstring& strText);		// ����Html�����ַ�

tstring GetBetweenString(const TCHAR* pStr, TCHAR cStart, TCHAR cEnd);
int GetBetweenInt(const TCHAR* pStr, TCHAR cStart, TCHAR cEnd, int nDefValue = 0);

tstring GetBetweenString(const WCHAR* pStr, const WCHAR* pStart, const WCHAR* pEnd);
int GetBetweenInt(const WCHAR* pStr, const WCHAR* pStart, 
				  const WCHAR* pEnd, int nDefValue = 0);

BOOL DllRegisterServer(LPCTSTR lpszFileName);
BOOL DllUnregisterServer(LPCTSTR lpszFileName);

tstring GetMimeTypeByExtension(const TCHAR* lpExtension);
//GUID GetFileTypeGuidByExtension(const WCHAR* lpExtension);
//CLSID GetEncoderClsidByExtension(const WCHAR* lpExtension);
//CLSID GetEncoderClsidByFileType(REFGUID guidFileType);
//CLSID GetEncoderClsidByMimeType(const WCHAR* lpMimeType);

time_t httpdate_parse(const TCHAR*date);

BOOL CheckOnlyOneInstance();			//����������ʵ��

long GetFileCountOfSpecifiDirectory(PCSTR pszDirectoryName, PCTSTR pszFileExtension);	//��ȡָ���ļ���ĳ�������ļ�����

void SplitString(const CString& strSource, PCTSTR pszDelimiter, std::vector<CString>& aryStringArray);

DWORD GetNetcard(BYTE Address[8][6]);	//��ȡ��������

//HICON ExtractIcon(LPCTSTR lpszFileName, long nSize);

//BOOL GetImageWidthAndHeight(LPCTSTR lpszFileName, long& nWidth, long& nHeight);

//��⵱ǰ����״̬�ĺ���
BOOL IUIsNetworkAlive();
BOOL IUGetConnectedState();


//����ֵ���������ĸ߶Ⱥͷ���
//long GetTaskbarDirectionAndRect(CRect& rtTaskbar);

//��ͼƬת����ָ����ʽ
//int TransformImage(PCTSTR lpSrcFile, PCTSTR lpDstExt, long nDestWidth, long nDestHeight, CString& strDstFile);
//BOOL GetImageCLSID(PCTSTR format, CLSID* pCLSID);

BOOL CenterWindow2(HWND hTargetWnd, HWND hWndCenter = NULL);
//��һ����Ļλ��ת���ɴ���λ��
void ScreenToClient(HWND hwnd, RECT& rt);

BOOL IsMobileNumber(PCTSTR pszNumber);
