// majiangView.cpp : CmajiangView 类的实现
//

#include "stdafx.h"
#include "majiang.h"

#include "majiangDoc.h"
#include "majiangView.h"
#include "Mp3Sound.h"
#include "Cards.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


Mp3Sound m_mp3;
CCards m_card;


IMPLEMENT_DYNCREATE(CmajiangView, CView)

BEGIN_MESSAGE_MAP(CmajiangView, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()

	ON_MESSAGE(WM_ON_SENDCARD, OnSendMessage)
	ON_WM_TIMER()
END_MESSAGE_MAP()

// CmajiangView 构造/析构

CmajiangView::CmajiangView()
{


}

CmajiangView::~CmajiangView()
{
}

BOOL CmajiangView::PreCreateWindow(CREATESTRUCT& cs)
{
	
      m_card.initCards();
	return CView::PreCreateWindow(cs);
}


void CmajiangView::OnDraw(CDC* pDC)
{
	CmajiangDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	if ( m_card.GetMessage()&&m_card.getGameState()==GAME_START)
	{
		m_card.SetMessage(false);
		m_card.getPai();
		SetTimer(1,1000,0);

	}
	if (!pDoc)
		return;
	m_card.drawCards(pDC);

	// TODO: 在此处为本机数据添加绘制代码
}


// CmajiangView 打印

BOOL CmajiangView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CmajiangView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CmajiangView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清除过程
}


// CmajiangView 诊断

#ifdef _DEBUG
void CmajiangView::AssertValid() const
{
	CView::AssertValid();
}

void CmajiangView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CmajiangDoc* CmajiangView::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CmajiangDoc)));
	return (CmajiangDoc*)m_pDocument;
}
#endif //_DEBUG


// CmajiangView 消息处理程序

void CmajiangView::OnMouseMove(UINT nFlags, CPoint point)
{
	
	m_card.mouseMove(point);
	CRect m_clientRect;
	m_clientRect.left=0;
	m_clientRect.top=0;
	m_clientRect.right=900;
	m_clientRect.bottom=620;
	InvalidateRect(&m_clientRect,false);
	CView::OnMouseMove(nFlags, point);
}

void CmajiangView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	m_card.lbuttonDown(point);

	CRect m_clientRect;
	m_clientRect.left=0;
	m_clientRect.top=0;
	m_clientRect.right=900;
	m_clientRect.bottom=620;
	InvalidateRect(&m_clientRect,false);
	CView::OnLButtonDown(nFlags, point);
}
LRESULT CmajiangView::OnSendMessage(WPARAM wParam, LPARAM lParam)
{

	if (m_card.GetWhoSendCard()!=PLAYER_A&&m_card.getGameState()==GAME_START)
	{
		m_card.computerSendCards();

	}

	CRect m_clientRect;
	m_clientRect.left=0;
	m_clientRect.top=0;
	m_clientRect.right=900;
	m_clientRect.bottom=620;
	InvalidateRect(&m_clientRect,false);
	return 0;

}
void CmajiangView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
    if (nIDEvent==1)
    {
		KillTimer(1);
		SendMessage(WM_ON_SENDCARD);
    }
	CView::OnTimer(nIDEvent);
}
