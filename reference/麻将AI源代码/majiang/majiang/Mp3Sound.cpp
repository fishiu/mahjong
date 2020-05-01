
#include "stdafx.h"
#include "Mp3Sound.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

Mp3Sound::Mp3Sound()
{
	//
	//TCHAR szFilePath[MAX_PATH + 1];
	//GetModuleFileName(NULL, szFilePath, MAX_PATH);
	//(_tcsrchr(szFilePath, '\\'))[1] = '\0'; //删除文件名，只获得路径
	//strcpy(szFilePath+strlen(szFilePath), "sound\\backmusic.mp3");
	//Open(szFilePath);
}

Mp3Sound::~Mp3Sound()
{
	Close();
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Open
* 函数介绍：用mci命令打开MP3文件
* 输入参数：
            CString strPath, MP3文件的路径
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Open(const CString &strPath)
{
	MCI_OPEN_PARMS mciOP;
	DWORD dwReturn;

	CString str;
	::GetCurrentDirectory(MAX_PATH,str.GetBuffer(MAX_PATH));
	str.ReleaseBuffer();
	str+="\\sound\\";
	str+=strPath;
	ASSERT(m_wID == NULL);
	//由mci判断文件类型
	mciOP.lpstrDeviceType=NULL;
	mciOP.lpstrElementName=str;
	dwReturn=mciSendCommand(NULL, MCI_OPEN, MCI_OPEN_ELEMENT | MCI_WAIT | MCI_OPEN_SHAREABLE,
		(DWORD)(LPVOID)&mciOP);
	if (dwReturn==0)
	{	m_wID=mciOP.wDeviceID;
		if (this->GetLength(&m_dwLength)!=0)
			m_dwLength=0;
	}
	else
		m_wID=NULL;
	return dwReturn;

}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Close
* 函数介绍：用mci命令关闭当前打开的MP3文件
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Close()
{
	MCI_GENERIC_PARMS mciGP;
	DWORD dwReturn;

	dwReturn = mciSendCommand(m_wID, MCI_CLOSE, MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
	m_wID = NULL;
	return dwReturn;
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Play
* 函数介绍：用mci命令播放已打开（当前）的MP3文件
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Play()
{
	MCI_PLAY_PARMS mciPP;
	return mciSendCommand(m_wID, MCI_PLAY, MCI_NOTIFY,
		(DWORD)(LPVOID)&mciPP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Pause
* 函数介绍：用mci命令暂停播放
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Pause()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID, MCI_PAUSE, MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Resume
* 函数介绍：用mci命令恢复播放
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Resume()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID, MCI_RESUME, MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：Stop
* 函数介绍：用mci命令停止播放（效果与Pause相同）
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::Stop()
{
	MCI_GENERIC_PARMS mciGP;
	return mciSendCommand(m_wID, MCI_STOP, MCI_NOTIFY|MCI_WAIT,
		(DWORD)(LPVOID)&mciGP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：SeekTo
* 函数介绍：用mci命令跳到指定位置
* 输入参数：
*           DWORD dwTo, 指定位置，单位为秒
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::SeekTo(DWORD dwTo)
{
	DWORD dwReturn;
	if (dwTo>0 && dwTo<m_dwLength)
	{	MCI_SEEK_PARMS mciSP;
		mciSP.dwTo=dwTo*1000;
		dwReturn = mciSendCommand(m_wID, MCI_SEEK, MCI_WAIT|MCI_NOTIFY|MCI_TO,
			(DWORD)(LPVOID)&mciSP);
	}else if (dwTo>=m_dwLength)
		dwReturn = SeekToEnd();
	else
		dwReturn = SeekToStart();
	return dwReturn;
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：SeekToStart
* 函数介绍：用mci命令跳到当前MP3文件开始处
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::SeekToStart()
{
	MCI_SEEK_PARMS mciSP;
	return mciSendCommand(m_wID, MCI_SEEK, MCI_WAIT|MCI_NOTIFY|MCI_SEEK_TO_START,
		(DWORD)(LPVOID)&mciSP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：SeekToEnd
* 函数介绍：跳到当前MP3文件末尾处
* 输入参数：无
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::SeekToEnd()
{
	MCI_SEEK_PARMS mciSP;
	return mciSendCommand(m_wID, MCI_SEEK, MCI_WAIT|MCI_NOTIFY|MCI_SEEK_TO_END,
		(DWORD)(LPVOID)&mciSP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：GetLength
* 函数介绍：用mci命令取得当前打开的MP3文件的长度
* 输入参数：无
* 输出参数：
*           DWORD *dwLength, 输出MP3文件的长度，单位为秒
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::GetLength(DWORD *dwLength)
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_LENGTH;
	DWORD dwReturn=mciSendCommand(m_wID, MCI_STATUS, MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if (dwReturn==0)
		*dwLength=mciSP.dwReturn/1000;
	return dwReturn;
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：GetPos
* 函数介绍：用mci命令取得当前打开的MP3文件的当前位置
* 输入参数：无
* 输出参数：
*           DWORD *dwPos, 输出MP3文件的当前位置，单位为秒
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::GetPos(DWORD *dwPos)
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_POSITION;
	DWORD dwReturn=mciSendCommand(m_wID, MCI_STATUS, MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if (dwReturn==0)
		*dwPos=mciSP.dwReturn/1000;
	return dwReturn;
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：SetMute
* 函数介绍：用mci命令设置静音或取消静音
* 输入参数：
*           BOOL bMute, 设置静音(bMute=true)或取消静音(bMute=false)
* 输出参数：无
* 返回值  ：mci命令的错误信息
*/
DWORD Mp3Sound::SetMute(BOOL bMute)
{
	MCI_SET_PARMS mciSP;
	mciSP.dwAudio=MCI_SET_AUDIO_ALL;
	DWORD dwFlags;
	if (bMute)
		dwFlags=MCI_WAIT|MCI_SET_AUDIO|MCI_SET_OFF;
	else
		dwFlags=MCI_WAIT|MCI_SET_AUDIO|MCI_SET_ON;
	return mciSendCommand(m_wID, MCI_SET, dwFlags,
		(DWORD)(LPVOID)&mciSP);
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：IsPlaying
* 函数介绍：用mci命令询问是否正在播放
* 输入参数：无
* 输出参数：无
* 返回值  ：BOOL, 返回正在播放(true)或不是正在播放(false)
*/
BOOL Mp3Sound::IsPlaying()
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_MODE;
	DWORD dwReturn=mciSendCommand(m_wID, MCI_STATUS, MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if (dwReturn==0)
		if (mciSP.dwReturn==MCI_MODE_PLAY)
			return true;
	return false;
}

////////////////////////////////////////////////////////////////////////
/*
* 函数名称：IsPlaying
* 函数介绍：用mci命令询问有否文件打开
* 输入参数：无
* 输出参数：无
* 返回值  ：BOOL, 返回已有文件打开(true)或没有文件打开(false)
*/
BOOL Mp3Sound::IsOpened()
{
	MCI_STATUS_PARMS mciSP;
	mciSP.dwItem=MCI_STATUS_MODE;
	DWORD dwReturn=mciSendCommand(m_wID, MCI_STATUS, MCI_STATUS_ITEM,
		(DWORD)(LPVOID)&mciSP);
	if (dwReturn==(DWORD)0)
		if (mciSP.dwReturn==MCI_MODE_OPEN ||
			mciSP.dwReturn==MCI_MODE_PLAY ||
			mciSP.dwReturn==MCI_MODE_PAUSE ||
			mciSP.dwReturn==MCI_MODE_STOP ||
			mciSP.dwReturn==MCI_MODE_SEEK
			)
			return true;
	return false;
}


