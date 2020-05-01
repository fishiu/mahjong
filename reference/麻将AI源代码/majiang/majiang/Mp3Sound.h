
#if !defined(AFX_MCIMP3_H__31175A6D_4435_41BF_BB02_5D78903D7338__INCLUDED_)
#define AFX_MCIMP3_H__31175A6D_4435_41BF_BB02_5D78903D7338__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#pragma comment(lib,"Wininet.lib") 
#pragma comment(lib,"Winmm.lib") 
#include <mmsystem.h>

class Mp3Sound  
{
public:
	Mp3Sound();
	virtual ~Mp3Sound();
	DWORD Open(const CString &strPath);
	DWORD Close();
	DWORD Play();
	DWORD Pause();
	DWORD Resume();
	DWORD Stop();
	DWORD SetMute(BOOL bMute);
	DWORD GetLength(DWORD *dwReturn);
	DWORD GetPos(DWORD *dwPos);
	DWORD SeekTo(DWORD dwTo);
	DWORD SeekToStart();
	DWORD SeekToEnd();
	BOOL IsPlaying();
	BOOL IsOpened();
	DWORD m_dwLength;
	//MCI…Ë±∏ID
	MCIDEVICEID m_wID;
};

#endif // !defined(AFX_MCIMP3_H__31175A6D_4435_41BF_BB02_5D78903D7338__INCLUDED_)


