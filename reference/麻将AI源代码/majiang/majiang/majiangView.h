// majiangView.h : CmajiangView 类的接口
//


#pragma once

#define WM_ON_SENDCARD  (WM_USER+103)
class CmajiangView : public CView
{
protected: // 仅从序列化创建
	CmajiangView();
	DECLARE_DYNCREATE(CmajiangView)

// 属性
public:
	CmajiangDoc* GetDocument() const;

// 操作
public:

// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CmajiangView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
public:
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnSendMessage(WPARAM wParam, LPARAM lParam); 
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};

#ifndef _DEBUG  // majiangView.cpp 中的调试版本
inline CmajiangDoc* CmajiangView::GetDocument() const
   { return reinterpret_cast<CmajiangDoc*>(m_pDocument); }
#endif

