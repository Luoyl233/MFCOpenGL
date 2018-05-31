
// MainFrm.cpp : CMainFrame 类的实现
//

#include "stdafx.h"
#include "MFCOpenGL.h"

#include "MainFrm.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

// CMainFrame

IMPLEMENT_DYNCREATE(CMainFrame, CFrameWnd)

BEGIN_MESSAGE_MAP(CMainFrame, CFrameWnd)
	ON_WM_CREATE()
END_MESSAGE_MAP()

static UINT indicators[] =
{
	ID_SEPARATOR,           // 状态行指示器
	ID_INDICATOR_CAPS,
	ID_INDICATOR_NUM,
	ID_INDICATOR_SCRL,
};

// CMainFrame 构造/析构

CMainFrame::CMainFrame()
{
	// TODO: 在此添加成员初始化代码
}

CMainFrame::~CMainFrame()
{
}

int CMainFrame::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CFrameWnd::OnCreate(lpCreateStruct) == -1)
		return -1;
	
	if (!m_wndToolBar.CreateEx(this, TBSTYLE_FLAT, WS_CHILD | WS_VISIBLE | CBRS_TOP | CBRS_GRIPPER | CBRS_TOOLTIPS | CBRS_FLYBY | CBRS_SIZE_DYNAMIC) ||
		!m_wndToolBar.LoadToolBar(IDR_MAINFRAME))
	{
		TRACE0("未能创建工具栏\n");
		return -1;      // 未能创建
	}

	//m_wndToolBar.Create(this);
	//m_wndToolBar.LoadToolBar(IDR_MAINFRAME);
//	m_wndReBar.Create(this);
//	m_wndReBar.AddBar(&m_wndToolBar, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_FIXEDBMP | RBBS_USECHEVRON);
	/*
	CRect rectToolBarItem;
	m_wndToolBar.GetItemRect(0, &rectToolBarItem);
	REBARBANDINFO rbbi;
	rbbi.cbSize = sizeof(rbbi);
	rbbi.fMask = RBBIM_CHILDSIZE | RBBIM_IDEALSIZE | RBBIM_SIZE | RBBIM_ID;
	rbbi.cxMinChild = rectToolBarItem.Width();
	rbbi.cyMinChild = rectToolBarItem.Height();
	rbbi.cx = rbbi.cxIdeal = rectToolBarItem.Width() * 3;
	rbbi.wID = 0;
	m_wndReBar.GetReBarCtrl().SetBandInfo(0, &rbbi);
	*/
	//m_editR.Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 30, 20), this, 0);
//	m_editG.Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 30, 20), this, 0);
//	m_editB.Create(WS_CHILD|WS_VISIBLE, CRect(0, 0, 30, 20), this, 0);
	

	//m_wndReBar.AddBar(&m_staticR, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_USECHEVRON | RBBS_FIXEDSIZE);
	//m_wndReBar.AddBar(&m_editR, _T("R:"), NULL, RBBS_NOGRIPPER);
	//m_wndReBar.AddBar(&m_staticG, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_USECHEVRON | RBBS_FIXEDSIZE);
	//m_wndReBar.AddBar(&m_editG, _T("G:"), NULL, RBBS_NOGRIPPER);
	//m_wndReBar.AddBar(&m_staticB, NULL, NULL, RBBS_GRIPPERALWAYS | RBBS_USECHEVRON | RBBS_FIXEDSIZE);
//	m_wndReBar.AddBar(&m_editB, _T("B:"), NULL, RBBS_NOGRIPPER);

	//m_wndReBar.SetWindowPos(NULL, 0, 0, 260, 32, SWP_SHOWWINDOW);

//	m_wndToolBar.CreateEx(this, TBSTYLE_FLAT|TBSTYLE_TRANSPARENT);


	if (!m_wndStatusBar.Create(this))
	{
		TRACE0("未能创建状态栏\n");
		return -1;      // 未能创建
	}
	m_wndStatusBar.SetIndicators(indicators, sizeof(indicators)/sizeof(UINT));

	// TODO: 如果不需要可停靠工具栏，则删除这三行
	m_wndToolBar.EnableDocking(CBRS_ALIGN_ANY);
	EnableDocking(CBRS_ALIGN_ANY);
	DockControlBar(&m_wndToolBar);


	return 0;
}

BOOL CMainFrame::PreCreateWindow(CREATESTRUCT& cs)
{
	if( !CFrameWnd::PreCreateWindow(cs) )
		return FALSE;
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	cs.style = WS_OVERLAPPED | WS_CAPTION | FWS_ADDTOTITLE
		 | WS_THICKFRAME | WS_MINIMIZEBOX | WS_SYSMENU;
	//取消最大化 && 固定窗口尺寸
	cs.style &= ~WS_MAXIMIZEBOX;
	cs.style &= ~WS_THICKFRAME;
	/*
	cs.lpszName = _T("painter");
	cs.cx = 600;
	cs.cy = 600;
	*/

	return TRUE;
}

// CMainFrame 诊断

#ifdef _DEBUG
void CMainFrame::AssertValid() const
{
	CFrameWnd::AssertValid();
}

void CMainFrame::Dump(CDumpContext& dc) const
{
	CFrameWnd::Dump(dc);
}
#endif //_DEBUG


// CMainFrame 消息处理程序

