
// MFCOpenGLView.h : CMFCOpenGLView 类的接口
//

#pragma once
#include "MFCOpenGLDoc.h"
#include <list>
#include <algorithm>
#include "GL/gl.h"
#include "GL/glu.h"
#include "GL/glut.h"
#include "Shape/Graph.h"
#include "Shape/GLine.h"
#include "Shape/GCircle.h"
#include "Shape/GEllipse.h"
#include "Shape/GPolygon.h"
#include "Shape\pixels.h"
#include "DrawTool.h"
#include "RotateDlg.h"

//using namespace std;




#define GLxi(x)	((GLint)((x) - m_cx/2))
#define GLyi(y)	((GLint)(-(y) + m_cy/2))
#define GLpi(point) CPoint(((GLint)((point.x) - m_cx/2)), ((GLint)(-(point.y) + m_cy/2)))

#define GLxf(x)	((GLfloat)(2*(x) - m_cx) / (GLfloat)(m_cx))
#define GLyf(y)	((GLfloat)(-2*(y) + m_cy) / (GLfloat)(m_cy))


//定义当前状态
enum STATUS {
	NONE = 0,
	DRAW_LINE,		//画线
	DRAW_CIRCLE,		//画圆
	DRAW_ELLIPSE,	//画椭圆
	DRAW_CURVE,		//画曲线
	DRAW_POLYGON,		//画多边形
	SELECT,				//选取一个图形
	NUM_STATUS					//最后统计枚举的元素个数
};


class CMFCOpenGLView : public CView
{
protected: // 仅从序列化创建
	CMFCOpenGLView();
	DECLARE_DYNCREATE(CMFCOpenGLView)

// 特性
public:
	CMFCOpenGLDoc* GetDocument() const;

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
	virtual ~CMFCOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	void RenderScene();
	BOOL SetWindowPixelFormat(HDC hDC);//设置像素格式  
	BOOL CreateViewGLContext(HDC hDC);//创建绘制环境(RC)并使之成为当前绘制环境  
	BOOL InitGL(GLvoid);//初始化openGL  
	//int DrawGLScene(GLvoid);//绘图代码区  
	int m_GLPixelIndex;
	HGLRC m_hGLContext;//绘制环境  
	//{{AFX_MSG(CMy2OpenGLView)  
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//}}AFX_MSG 


	DECLARE_MESSAGE_MAP()

	//about graph
public:
	Graph *m_current;
	STATUS m_currentStatus;
	int m_cx, m_cy;
	BOOL m_btnDown;

	GLIST m_selection;			//已选择的图元
	COLORREF m_lineColor;
	COLORREF m_fillColor;

	CRect m_clipRect;
	bool m_drawClipRect;

	void setClipRect(CRect &rect);

	GLIST &getObjects();
	void addObject(Graph *obj);
	void select(Graph *obj);
	Graph *objAt(const CPoint &point);

public:
	afx_msg void OnDrawline();
	afx_msg void OnDrawellipse();
	afx_msg void OnDrawcurve();
	afx_msg void OnDrawpolygon();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnDrawcircle();
	afx_msg void OnEditUndo();
	afx_msg void OnClear();
	afx_msg void OnSelect();
	afx_msg void OnLinecolor();
	afx_msg void OnFillcolor();
	afx_msg void OnClip();
	afx_msg void OnRotate();
	afx_msg void OnMulticurve();
	afx_msg void OnDrawmulticurve();
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnDraw3d();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg void OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnDrawrect();
	afx_msg void OnDrawfill();
	afx_msg void OnDelete();
	afx_msg void OnUpdateSelect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateClip(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawline(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawrect(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawcircle(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawellipse(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawpolygon(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawfill(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawcurve(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDrawmulticurve(CCmdUI *pCmdUI);
	afx_msg void OnUpdateDraw3d(CCmdUI *pCmdUI);
};

#ifndef _DEBUG  // MFCOpenGLView.cpp 中的调试版本
inline CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLDoc*>(m_pDocument); }
#endif

