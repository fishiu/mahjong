// majiangDoc.cpp : CmajiangDoc 类的实现
//

#include "stdafx.h"
#include "majiang.h"

#include "majiangDoc.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CmajiangDoc

IMPLEMENT_DYNCREATE(CmajiangDoc, CDocument)

BEGIN_MESSAGE_MAP(CmajiangDoc, CDocument)
END_MESSAGE_MAP()


// CmajiangDoc 构造/析构

CmajiangDoc::CmajiangDoc()
{
	// TODO: 在此添加一次性构造代码

}

CmajiangDoc::~CmajiangDoc()
{
}

BOOL CmajiangDoc::OnNewDocument()
{
	if (!CDocument::OnNewDocument())
		return FALSE;

	// TODO: 在此添加重新初始化代码
	// (SDI 文档将重用该文档)

	return TRUE;
}




// CmajiangDoc 序列化

void CmajiangDoc::Serialize(CArchive& ar)
{
	if (ar.IsStoring())
	{
		// TODO: 在此添加存储代码
	}
	else
	{
		// TODO: 在此添加加载代码
	}
}


// CmajiangDoc 诊断

#ifdef _DEBUG
void CmajiangDoc::AssertValid() const
{
	CDocument::AssertValid();
}

void CmajiangDoc::Dump(CDumpContext& dc) const
{
	CDocument::Dump(dc);
}
#endif //_DEBUG


// CmajiangDoc 命令
