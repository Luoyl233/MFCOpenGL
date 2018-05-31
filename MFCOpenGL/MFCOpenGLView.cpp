
// MFCOpenGLView.cpp : CMFCOpenGLView ���ʵ��
//

#include "stdafx.h"
// SHARED_HANDLERS ������ʵ��Ԥ��������ͼ������ɸѡ�������
// ATL ��Ŀ�н��ж��壬�����������Ŀ�����ĵ����롣
#ifndef SHARED_HANDLERS
#include "MFCOpenGL.h"
#endif

#include "MFCOpenGLDoc.h"
#include "MFCOpenGLView.h"

#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCOpenGLView

IMPLEMENT_DYNCREATE(CMFCOpenGLView, CView)

BEGIN_MESSAGE_MAP(CMFCOpenGLView, CView)
	//{{AFX_MSG_MAP(CMy2OpenGLView)  
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	//}}AFX_MSG_MAP  
	// ��׼��ӡ����
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_COMMAND(ID_DRAWLINE, &CMFCOpenGLView::OnDrawline)
	ON_COMMAND(ID_DRAWELLIPSE, &CMFCOpenGLView::OnDrawellipse)
	ON_COMMAND(ID_DRAWCURVE, &CMFCOpenGLView::OnDrawcurve)
	ON_COMMAND(ID_DRAWPOLYGON, &CMFCOpenGLView::OnDrawpolygon)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_COMMAND(ID_DRAWCIRCLE, &CMFCOpenGLView::OnDrawcircle)
	ON_COMMAND(ID_EDIT_UNDO, &CMFCOpenGLView::OnEditUndo)
	ON_COMMAND(ID_CLEAR, &CMFCOpenGLView::OnClear)
	ON_COMMAND(ID_SELECT, &CMFCOpenGLView::OnSelect)
	ON_COMMAND(ID_LINECOLOR, &CMFCOpenGLView::OnLinecolor)
	ON_COMMAND(ID_FILLCOLOR, &CMFCOpenGLView::OnFillcolor)
	ON_COMMAND(ID_CLIP, &CMFCOpenGLView::OnClip)
	ON_COMMAND(ID_ROTATE, &CMFCOpenGLView::OnRotate)
	ON_COMMAND(ID_DRAWMULTICURVE, &CMFCOpenGLView::OnDrawmulticurve)
	ON_WM_LBUTTONDBLCLK()
	ON_COMMAND(ID_DRAW3D, &CMFCOpenGLView::OnDraw3d)
	ON_WM_KEYDOWN()
	ON_WM_KEYUP()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_DRAWRECT, &CMFCOpenGLView::OnDrawrect)
	ON_COMMAND(ID_DRAWFILL, &CMFCOpenGLView::OnDrawfill)
	ON_COMMAND(ID_DELETE, &CMFCOpenGLView::OnDelete)
	ON_UPDATE_COMMAND_UI(ID_SELECT, &CMFCOpenGLView::OnUpdateSelect)
	ON_UPDATE_COMMAND_UI(ID_CLIP, &CMFCOpenGLView::OnUpdateClip)
	ON_UPDATE_COMMAND_UI(ID_DRAWLINE, &CMFCOpenGLView::OnUpdateDrawline)
	ON_UPDATE_COMMAND_UI(ID_DRAWRECT, &CMFCOpenGLView::OnUpdateDrawrect)
	ON_UPDATE_COMMAND_UI(ID_DRAWCIRCLE, &CMFCOpenGLView::OnUpdateDrawcircle)
	ON_UPDATE_COMMAND_UI(ID_DRAWELLIPSE, &CMFCOpenGLView::OnUpdateDrawellipse)
	ON_UPDATE_COMMAND_UI(ID_DRAWPOLYGON, &CMFCOpenGLView::OnUpdateDrawpolygon)
	ON_UPDATE_COMMAND_UI(ID_DRAWFILL, &CMFCOpenGLView::OnUpdateDrawfill)
	ON_UPDATE_COMMAND_UI(ID_DRAWCURVE, &CMFCOpenGLView::OnUpdateDrawcurve)
	ON_UPDATE_COMMAND_UI(ID_DRAWMULTICURVE, &CMFCOpenGLView::OnUpdateDrawmulticurve)
	ON_UPDATE_COMMAND_UI(ID_DRAW3D, &CMFCOpenGLView::OnUpdateDraw3d)
END_MESSAGE_MAP()

// CMFCOpenGLView ����/����

CMFCOpenGLView::CMFCOpenGLView()
	:m_clipRect(0, 0, 0, 0)
{
	// TODO: �ڴ˴���ӹ������
	this->m_GLPixelIndex = 0;
	this->m_hGLContext = NULL;

	m_btnDown = FALSE;

	/*
	graph[DRAW_LINE] = new GLine();	
	graph[DRAW_CIRCLE] = new GCircle();
	graph[DRAW_ELLIPSE] = new GEllipse();
	graph[DRAW_CURVE] = new GLine();
	graph[DRAW_POLYGON] = new GLine();
	*/

	m_currentStatus = STATUS::NONE;
	m_current = NULL;
	m_lineColor = RGB(0, 0, 0);
	m_fillColor = RGB(0, 0, 0);

	m_drawClipRect = false;
}

CMFCOpenGLView::~CMFCOpenGLView()
{
}

BOOL CMFCOpenGLView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: �ڴ˴�ͨ���޸�
	//  CREATESTRUCT cs ���޸Ĵ��������ʽ
	cs.style |= (WS_CLIPCHILDREN | WS_CLIPSIBLINGS);//openGL�����

	return CView::PreCreateWindow(cs);
}

// CMFCOpenGLView ����

void CMFCOpenGLView::OnDraw(CDC* pDC)
{
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: �ڴ˴�Ϊ����������ӻ��ƴ���
	::glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	RenderScene();
	// Tell OpenGL to flush its pipeline  
	::glFinish();
	// Now Swap the buffers  
	::SwapBuffers(pDC->GetSafeHdc());
}


// CMFCOpenGLView ��ӡ

BOOL CMFCOpenGLView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// Ĭ��׼��
	return DoPreparePrinting(pInfo);
}

void CMFCOpenGLView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӷ���Ĵ�ӡǰ���еĳ�ʼ������
}

void CMFCOpenGLView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: ��Ӵ�ӡ����е��������
}


// CMFCOpenGLView ���

#ifdef _DEBUG
void CMFCOpenGLView::AssertValid() const
{
	CView::AssertValid();
}

void CMFCOpenGLView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCOpenGLDoc* CMFCOpenGLView::GetDocument() const // �ǵ��԰汾��������
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCOpenGLDoc)));
	return (CMFCOpenGLDoc*)m_pDocument;
}
#endif //_DEBUG


BOOL CMFCOpenGLView::SetWindowPixelFormat(HDC hDC)
{
	//���崰�ڵ����ظ�ʽ  
	PIXELFORMATDESCRIPTOR pixelDesc =
	{
		sizeof(PIXELFORMATDESCRIPTOR),
		1,
		PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL |
		PFD_DOUBLEBUFFER | PFD_SUPPORT_GDI,
		PFD_TYPE_RGBA,
		24,
		0,0,0,0,0,0,
		0,
		0,
		0,
		0,0,0,0,
		32,
		0,
		0,
		PFD_MAIN_PLANE,
		0,
		0,0,0
	};

	this->m_GLPixelIndex = ChoosePixelFormat(hDC, &pixelDesc);//ѡ������������ظ�ʽ  
	if (this->m_GLPixelIndex == 0)
	{//ѡ��ʧ��  
		this->m_GLPixelIndex = 1;//Ĭ�ϵ����ظ�ʽ  
		if (DescribePixelFormat(hDC, this->m_GLPixelIndex, sizeof(PIXELFORMATDESCRIPTOR), &pixelDesc) == 0)
		{//��Ĭ�ϵ����ظ�ʽ��������  
			return FALSE;
		}
	}

	if (SetPixelFormat(hDC, this->m_GLPixelIndex, &pixelDesc) == FALSE)
	{
		return FALSE;
	}
	return TRUE;
}

BOOL CMFCOpenGLView::InitGL(GLvoid)                                        // All Setup For OpenGL Goes Here  
{
	glShadeModel(GL_SMOOTH);                            // Enable Smooth Shading  
	glClearColor(1.0, 1.0, 1.0, 0.0);// Black Background  
	glClearDepth(1.0f);                                    // Depth Buffer Setup  
	glEnable(GL_DEPTH_TEST);                            // Enables Depth Testing  
	glDepthFunc(GL_LEQUAL);                                // The Type Of Depth Testing To Do  
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);    // Really Nice Perspective Calculations  
	return TRUE;                                        // Initialization Went OK  
}

BOOL CMFCOpenGLView::CreateViewGLContext(HDC hDC)
{
	this->m_hGLContext = wglCreateContext(hDC);//����RC  
	if (this->m_hGLContext == NULL)
	{//����ʧ��  
		return FALSE;
	}

	if (wglMakeCurrent(hDC, this->m_hGLContext) == FALSE)
	{//ѡΪ��ǰRCʧ��  
		return FALSE;
	}
	return TRUE;
}


int CMFCOpenGLView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;

	// TODO: Add your specialized creation code here  
	HWND hWnd = this->GetSafeHwnd();
	HDC hDC = ::GetDC(hWnd);


	if (this->SetWindowPixelFormat(hDC) == FALSE)
	{//�������ظ�ʽ  
		return 0;
	}
	if (this->CreateViewGLContext(hDC) == FALSE)
	{//����RC��ѡΪ����  
		return 0;
	}
	if (!this->InitGL())
	{//��ʼ��openGL  
		return 0;
	}
	return 0;
}

BOOL CMFCOpenGLView::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��ֹwindows����ɫ����˸
	return true;
}

void CMFCOpenGLView::OnDestroy()
{
	CView::OnDestroy();

	// TODO: Add your message handler code here  
	if (wglGetCurrentContext() != NULL)
	{
		wglMakeCurrent(NULL, NULL);
	}
	if (this->m_hGLContext != NULL)
	{
		wglDeleteContext(this->m_hGLContext);
		this->m_hGLContext = NULL;
	}

	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GLIST &graphList = pDoc->m_graphList;
	
	//�ͷŶ�̬�����ռ�
	GLIST::iterator iter = graphList.begin();
	while (iter != graphList.end()) {
		Graph* tmp = *iter;
		delete tmp;
		graphList.erase(iter++);	
	}
	m_selection.clear();
	if (m_current != NULL) delete m_current;

}

void CMFCOpenGLView::OnSize(UINT nType, int cx, int cy)
{
	CView::OnSize(nType, cx, cy);

	// TODO: Add your message handler code here  
	GLdouble aspect_ratio; // width/height ratio  

	if (0 >= cx || 0 >= cy)
	{
		return;
	}
	// select the full client area  
	::glViewport(0, 0, cx, cy);
	m_cx = cx;
	m_cy = cy;
	// compute the aspect ratio  
	// this will keep all dimension scales equal  
	aspect_ratio = (GLdouble)cx / (GLdouble)cy;
	// select the projection matrix and clear it  
	::glMatrixMode(GL_PROJECTION);
	::glLoadIdentity();
	// select the viewing volume  
	//  gluOrtho2D(-10.0f,10.0f,-10.0f,10.0f);  
	//gluOrtho2D(-cx / 2, cx / 2, -cy / 2, cy / 2); // select the viewing volume
	glOrtho(-cx / 2, cx / 2, -cy / 2, cy / 2, -cx / 2, cx / 2);
	//::gluPerspective(45.0f, aspect_ratio, 0.01f, 200.0f);
	//::gluPerspective(45.0f, aspect_ratio, 10.0f, 100000.0f);

	// switch back to the modelview matrix and clear it  
	::glMatrixMode(GL_MODELVIEW);
	::glLoadIdentity();                              // Reset The Modelview Matrix  
}

void CMFCOpenGLView::RenderScene()
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);    // Clear Screen And Depth Buffer  
	glColor3f(0.0f, 0.0f, 0.0f);						//������ɫ
	glPolygonMode(GL_FRONT, GL_LINE);					//���ö���ε��������Լ����Ʒ�ʽ
	//glLoadIdentity();	// Reset The Current Modelview Matrix  
	//glTranslatef(-1.0f, 0.0f, -5.0f);//��������1.5,������6,�൱���ƶ���ͷһ������������뾵ͷ�� 
	
	clearPixel(RGB(255, 255, 255));

	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	GLIST &graphList = pDoc->m_graphList;

	GLIST::iterator iter = graphList.begin();
	while (iter != graphList.end()) {
		(*iter++)->draw();
	}

	if (m_selection.size() == 1) {
		m_selection.front()->drawExRect();
		m_selection.front()->drawHandle();
	}

	if (m_drawClipRect) {
		//draw clip rect
		m_drawClipRect = false;
		glColor3f(1.0, 0.0, 0.0);
		glLineWidth(1.0);
		glLineStipple(1, 0x0F0F);
		glEnable(GL_LINE_STIPPLE);

		glBegin(GL_LINES);
		glVertex2i(m_clipRect.left, m_clipRect.top);
		glVertex2i(m_clipRect.left, m_clipRect.bottom);
		glVertex2i(m_clipRect.right, m_clipRect.bottom);
		glVertex2i(m_clipRect.right, m_clipRect.top);
		glVertex2i(m_clipRect.left, m_clipRect.top);
		glVertex2i(m_clipRect.right, m_clipRect.top);
		glVertex2i(m_clipRect.left, m_clipRect.bottom);
		glVertex2i(m_clipRect.right, m_clipRect.bottom);
		glEnd();
		glDisable(GL_LINE_STIPPLE);
	}


	glFlush();

}















// CMFCOpenGLView ��Ϣ�������


void CMFCOpenGLView::setClipRect(CRect & rect)
{
	this->m_clipRect = rect;
	m_drawClipRect = true;
}

GLIST & CMFCOpenGLView::getObjects()
{
	// TODO: �ڴ˴����� return ���
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);

	return pDoc->m_graphList;
}

void CMFCOpenGLView::addObject(Graph *obj)
{
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	pDoc->m_graphList.push_back(obj);
}

void CMFCOpenGLView::select(Graph * obj)
{
	//m_selection.front()->m_state = GNone;
	m_selection.clear();
	if (obj == NULL)	return;
	m_selection.push_back(obj);
	//obj->m_state = GSelected;
}

Graph * CMFCOpenGLView::objAt(const CPoint & point)
{
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return NULL;
	GLIST &graphList = pDoc->m_graphList;


	GLIST::iterator iter = find_if(graphList.begin(), graphList.end(),
		[&](Graph *obj) {return obj->hitTest(point) >= 0; }
	);
	if (iter != graphList.end())	return *iter;
	return NULL;
}

void CMFCOpenGLView::OnDrawline()
{
	// TODO: �ڴ���������������
	m_currentStatus = STATUS::DRAW_LINE;		
	DrawTool::c_drawShape = line;
}

void CMFCOpenGLView::OnDrawrect()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = rect;
}

void CMFCOpenGLView::OnDrawcircle()
{
	// TODO: �ڴ���������������
	m_currentStatus = DRAW_CIRCLE;
	DrawTool::c_drawShape = circle;
}

void CMFCOpenGLView::OnDrawellipse()
{
	// TODO: �ڴ���������������
	m_currentStatus = STATUS::DRAW_ELLIPSE;
	DrawTool::c_drawShape = ellipse;
}


void CMFCOpenGLView::OnDrawcurve()
{
	// TODO: �ڴ���������������
	m_currentStatus = STATUS::DRAW_CURVE;
	DrawTool::c_drawShape = curve;
	m_current = NULL;
}


void CMFCOpenGLView::OnDrawpolygon()
{
	// TODO: �ڴ���������������
	m_currentStatus = STATUS::DRAW_POLYGON;
	DrawTool::c_drawShape = poly;
	m_current = NULL;
}

void CMFCOpenGLView::OnDrawfill()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = fill;
}

void CMFCOpenGLView::OnDrawmulticurve()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = multiCurve;
}


void CMFCOpenGLView::OnDraw3d()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = parallelepiped;
}


void CMFCOpenGLView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnLButtonDown(this, nFlags, GLpi(point));

	CView::OnLButtonDown(nFlags, point);
}


void CMFCOpenGLView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_btnDown = FALSE;
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnLButtonUp(this, nFlags, GLpi(point));

	Invalidate(false);

	CView::OnLButtonUp(nFlags, point);
}


void CMFCOpenGLView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnMouseMove(this, nFlags, GLpi(point));

	Invalidate(false);
	CView::OnMouseMove(nFlags, point);
}



void CMFCOpenGLView::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnLButtonDblClk(this, nFlags, GLpi(point));

	Invalidate(false);
	CView::OnLButtonDblClk(nFlags, point);
}

BOOL CMFCOpenGLView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnMouseWheel(this, nFlags, zDelta, GLpi(pt));
	Invalidate(false);

	return CView::OnMouseWheel(nFlags, zDelta, pt);
}

void CMFCOpenGLView::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnKeyDown(this, nChar, nRepCnt, nFlags);

	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}

void CMFCOpenGLView::OnKeyUp(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	DrawTool *tool = DrawTool::FindTool(DrawTool::c_drawShape);
	if (tool != NULL)	tool->OnKeyUp(this, nChar, nRepCnt, nFlags);

	CView::OnKeyUp(nChar, nRepCnt, nFlags);
}



//�������� C + z
void CMFCOpenGLView::OnEditUndo()
{
	// TODO: �ڴ���������������

	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	GLIST &graphList = pDoc->m_graphList;

	if (graphList.empty() == FALSE) {
		Graph *obj = graphList.back();
		m_selection.remove(obj);
		delete obj;
		graphList.pop_back();
	}
	Invalidate();
}


void CMFCOpenGLView::OnDelete()
{
	// TODO: �ڴ���������������
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	GLIST &graphList = pDoc->m_graphList;

	if (graphList.empty() == FALSE && m_selection.size() == 1) {
		Graph *obj = m_selection.front();
		m_selection.remove(obj);
		graphList.remove(obj);
		delete obj;
	}
	Invalidate();
}

void CMFCOpenGLView::OnClear()
{
	// TODO: �ڴ���������������
	CMFCOpenGLDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;
	GLIST &graphList = pDoc->m_graphList;

	GLIST::iterator iter = graphList.begin();
	while (iter != graphList.end()) {
		Graph* tmp = *iter;
		delete tmp;
		graphList.erase(iter++);
	}
	m_selection.clear();

	Invalidate();
}




void CMFCOpenGLView::OnSelect()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = selection;
}


void CMFCOpenGLView::OnLinecolor()
{
	// TODO: �ڴ���������������
	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	clrDlg.m_cc.rgbResult = m_lineColor;
	if (clrDlg.DoModal() == IDOK) {
		m_lineColor = clrDlg.m_cc.rgbResult;
		
	}
	if (m_selection.size() == 1) {
		m_selection.front()->setlineColor(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	}
}


void CMFCOpenGLView::OnFillcolor()
{
	// TODO: �ڴ���������������
	CColorDialog clrDlg;
	clrDlg.m_cc.Flags |= CC_FULLOPEN | CC_RGBINIT;
	clrDlg.m_cc.rgbResult = m_fillColor;
	if (clrDlg.DoModal() == IDOK) {
		m_fillColor = clrDlg.m_cc.rgbResult;

	}
	if (m_selection.size() == 1) {
		m_selection.front()->setfillColor(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
	}
}


void CMFCOpenGLView::OnClip()
{
	// TODO: �ڴ���������������
	DrawTool::c_drawShape = clip;
}


void CMFCOpenGLView::OnRotate()
{
	// TODO: �ڴ���������������
	CRotateDlg dlg;
	double angel = 0.0;

	if (dlg.DoModal() == IDOK) {
		angel = dlg.m_angel;
	}
	if (m_selection.size() == 1) {
		m_selection.front()->rotate(angel);
		Invalidate();
	}
}














/*
 *UI���֣���Ҫ��Ϊ��ֱ��
 */
void CMFCOpenGLView::OnUpdateSelect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == selection);
}



void CMFCOpenGLView::OnUpdateClip(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == clip);
}


void CMFCOpenGLView::OnUpdateDrawline(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == line);
}


void CMFCOpenGLView::OnUpdateDrawrect(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == rect);
}


void CMFCOpenGLView::OnUpdateDrawcircle(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == circle);
}


void CMFCOpenGLView::OnUpdateDrawellipse(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == ellipse);
}


void CMFCOpenGLView::OnUpdateDrawpolygon(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == poly);
}


void CMFCOpenGLView::OnUpdateDrawfill(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == fill);
}


void CMFCOpenGLView::OnUpdateDrawcurve(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == curve);
}


void CMFCOpenGLView::OnUpdateDrawmulticurve(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == multiCurve);
}


void CMFCOpenGLView::OnUpdateDraw3d(CCmdUI *pCmdUI)
{
	// TODO: �ڴ������������û����洦��������
	pCmdUI->SetRadio(DrawTool::c_drawShape == parallelepiped);
}
