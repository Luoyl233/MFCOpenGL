
// MFCOpenGLView.h : CMFCOpenGLView ��Ľӿ�
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


//���嵱ǰ״̬
enum STATUS {
	NONE = 0,
	DRAW_LINE,		//����
	DRAW_CIRCLE,		//��Բ
	DRAW_ELLIPSE,	//����Բ
	DRAW_CURVE,		//������
	DRAW_POLYGON,		//�������
	SELECT,				//ѡȡһ��ͼ��
	NUM_STATUS					//���ͳ��ö�ٵ�Ԫ�ظ���
};


class CMFCOpenGLView : public CView
{
protected: // �������л�����
	CMFCOpenGLView();
	DECLARE_DYNCREATE(CMFCOpenGLView)

// ����
public:
	CMFCOpenGLDoc* GetDocument() const;

// ����
public:

// ��д
public:
	virtual void OnDraw(CDC* pDC);  // ��д�Ի��Ƹ���ͼ
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// ʵ��
public:
	virtual ~CMFCOpenGLView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// ���ɵ���Ϣӳ�亯��
protected:
	void RenderScene();
	BOOL SetWindowPixelFormat(HDC hDC);//�������ظ�ʽ  
	BOOL CreateViewGLContext(HDC hDC);//�������ƻ���(RC)��ʹ֮��Ϊ��ǰ���ƻ���  
	BOOL InitGL(GLvoid);//��ʼ��openGL  
	//int DrawGLScene(GLvoid);//��ͼ������  
	int m_GLPixelIndex;
	HGLRC m_hGLContext;//���ƻ���  
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

	GLIST m_selection;			//��ѡ���ͼԪ
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

#ifndef _DEBUG  // MFCOpenGLView.cpp �еĵ��԰汾
inline CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const
   { return reinterpret_cast<CMFCOpenGLDoc*>(m_pDocument); }
#endif

