#include "stdafx.h"
#include "DrawTool.h"
#include <algorithm>


//static var init
CPoint DrawTool::c_down;
UINT DrawTool::c_nDownFlags;
CPoint DrawTool::c_last;
DrawShape DrawTool::c_drawShape = selection;
ToolVec DrawTool::c_tools;

bool DrawTool::c_xDown = false;
bool DrawTool::c_yDown = false;
bool DrawTool::c_zDown = false;

static SelectTool selectTool(selection);
static RectTool lineTool(line);
static RectTool rectTool(rect);
static RectTool circleTool(circle);
static RectTool ellipseTool(ellipse);
static RectTool fillTool(fill);
static PolyTool polyTool(poly);
static ClipTool clipTool(clip);
static CurveTool curveTool(curve);
static MultiCurveTool multiCurveTool(multiCurve);
static ParallTool parallTol(parallelepiped);

int nDragHandle;
CPoint lastPoint;
SelectMode sMode;


DrawTool::DrawTool()
{
}

DrawTool::DrawTool(DrawShape nDrawShape)
{
	m_drawShape = nDrawShape;
	c_tools.push_back(this);
}

DrawTool::~DrawTool()
{
}

void DrawTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	//pView->SetCaptured();
	c_down = point;
	c_nDownFlags = nFlags;
	c_last = point;
}

void DrawTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
}

void DrawTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	//LButton click 
	if (point == c_down)
		c_drawShape = selection;
}

void DrawTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	c_last = point;
	SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
}

void DrawTool::OnMouseWheel(CMFCOpenGLView * pView, UINT nFlags, short zDelta, CPoint & pt)
{

}

void DrawTool::OnKeyDown(CMFCOpenGLView * pView, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
		case 'x':
		case 'X':
			c_xDown = true;
			break;
		case 'y':
		case 'Y':
			c_yDown = true;
			break;
		case 'z':
		case 'Z':
			c_zDown = true;
			break;
	default:
		break;
	}
}

void DrawTool::OnKeyUp(CMFCOpenGLView * pView, UINT nChar, UINT nRepCnt, UINT nFlags)
{
	switch (nChar)
	{
	case 'x':
	case 'X':
		c_xDown = false;
		break;
	case 'y':
	case 'Y':
		c_yDown = false;
		break;
	case 'z':
	case 'Z':
		c_zDown = false;
		break;
	default:
		break;
	}
}

void DrawTool::OnEditProperties(CMFCOpenGLView * pView)
{
}

void DrawTool::OnCancel()
{
	c_drawShape = selection;
}

DrawTool * DrawTool::FindTool(DrawShape drawShape)
{
	ToolVec::iterator iter = find_if(c_tools.begin(), c_tools.end(), 
		[&](DrawTool *tool) {return tool->m_drawShape == drawShape; }
		);
	if (iter != c_tools.end()) return *iter;
	return NULL;
}




RectTool::RectTool()
{
}

RectTool::RectTool(DrawShape nDrawShape)
	:DrawTool(nDrawShape)
{
}


RectTool::~RectTool()
{
}

void RectTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool::OnLButtonDown(pView, nFlags, point);

	Graph *graph;
	CRect rt(point, CSize(0, 0));
	switch (c_drawShape)
	{
	case line:
		graph = new GLine(rt);
		break;
	case rect:
		graph = new GRect(rt);
		break;
	case circle:
		graph = new GCircle(rt);
		break;
	case ellipse:
		graph = new GEllipse(rt);
		break;
	case fill:
		graph = new GFill(rt);
		graph->setfillColor(GetRValue(pView->m_fillColor), GetGValue(pView->m_fillColor), GetBValue(pView->m_fillColor));
		break;
	default:
		ASSERT(false);
		break;
	}

	GLIST &graphList = pView->getObjects();


	graphList.push_back(graph);
	pView->select(graph);
	nDragHandle = LT;
	lastPoint = point;
	sMode = SSize;
}

void RectTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void RectTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (point == c_down && c_drawShape != fill) {
		GLIST &graphList = pView->getObjects();
		Graph *obj = graphList.back();
		graphList.remove(obj);
		pView->m_selection.remove(obj);
	}

	if (c_drawShape != fill) {
		DrawTool *tool = DrawTool::FindTool(selection);
		if (tool != NULL)	tool->OnLButtonUp(pView, nFlags, point);
	}
}

void RectTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool *tool = DrawTool::FindTool(selection);
	if (tool != NULL)	tool->OnMouseMove(pView, nFlags, point);
	else ASSERT(false);
}

void RectTool::OnEditProperties(CMFCOpenGLView * pView)
{
}

void RectTool::OnCancel()
{
}


SelectTool::SelectTool(DrawShape nDrawShape)
	:DrawTool(nDrawShape)
{
}


SelectTool::~SelectTool()
{
}

void SelectTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	CPoint local = point;
	Graph *obj;
	sMode = SNone;

	if (pView->m_selection.size() == 1) {
		obj = pView->m_selection.front();
		nDragHandle = obj->hitTest(local);
		if (nDragHandle >= LT)	sMode = SSize;
	}

	if (sMode == SNone) {
		obj = pView->objAt(local);
		//ASSERT(obj != NULL);
		if (obj != NULL) {
			sMode = SMove;
			pView->select(obj);
		}
	}

	lastPoint = point;
	DrawTool::OnLButtonDown(pView, nFlags, point);
}

void SelectTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void SelectTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (pView->m_selection.size() == 1) {
		int nHandle = pView->m_selection.front()->hitTest(point);
		if (nHandle < 0)	pView->m_selection.clear();
	}
	if (pView->m_selection.size() == 0) {
		Graph *obj = pView->objAt(point);
		if (obj != NULL) {
			c_drawShape = selection;
			sMode = SNone;
			pView->select(obj);
		}
	}
}

void SelectTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	Graph *obj;
	CPoint local = point;
	if (pView->m_selection.size() == 1 && nFlags & MK_LBUTTON) {
		obj = pView->m_selection.front();
		//nDragHandle = obj->hitTest(local);
		CPoint delta = local - lastPoint;
		if (sMode == SMove) {
			obj->moveGraph(delta);
		}
		else if (sMode == SSize) {
			obj->moveHandle((GHANDLE)nDragHandle, delta);
		}

	}

	lastPoint = local;
	DrawTool::OnMouseMove(pView, nFlags, point);
}

void SelectTool::OnEditProperties(CMFCOpenGLView * pView)
{
}

void SelectTool::OnCancel()
{
}


/*
/////////////////////////////////////////////////////////////////////////
*/


PolyTool::PolyTool()
{
	m_drawShape = poly;
	m_drawObj = NULL;
	c_tools.push_back(this);
}

PolyTool::PolyTool(DrawShape nDrawShape)
	:DrawTool(nDrawShape)
{
	m_drawObj = NULL;
}

PolyTool::~PolyTool()
{
}

void PolyTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj == NULL) {
		CRect rect(point, CSize(0, 0));
		m_drawObj = new GPolygon(rect);
		m_drawObj->m_ptVec.push_back(point);
		m_drawObj->m_current = point;
		pView->addObject(m_drawObj);
		pView->select(m_drawObj);
		m_drawObj->updateExRect();
	}
	else if(CRect(m_drawObj->m_ptVec.front()+CPoint(-9, -9),CSize(18, 18)).PtInRect(point)) {
		if (m_drawObj->m_ptVec.size() >= 3) {
			m_drawObj->m_state = GNone;
			m_drawObj = NULL;
			c_drawShape = selection;
		}
	}
	else {
		m_drawObj->m_ptVec.push_back(point);
		m_drawObj->m_current = point;
		m_drawObj->updateExRect();
	}
}

void PolyTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
}

void PolyTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
}

void PolyTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj != NULL) {
		m_drawObj->m_current = point;
		m_drawObj->updateExRect();
	}
}

void PolyTool::OnCancel()
{
}


/*
//////////////////////////////////////////////////////////////////////////
*/

ClipTool::ClipTool()
{
}

ClipTool::ClipTool(DrawShape nDrawShape):DrawTool(nDrawShape)
{
}

ClipTool::~ClipTool()
{
}

void ClipTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool::OnLButtonDown(pView, nFlags, point);
}

void ClipTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void ClipTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (c_down != point) {
		CRect clipRect(c_down, point);
		pView->setClipRect(clipRect);

		GLIST &gList = pView->getObjects();
		GLIST::iterator iter = gList.begin();
		while (iter != gList.end()) {
			if ((*iter)->onClip(clipRect) == false) {
				pView->m_selection.remove(*iter);
				iter = gList.erase(iter);
			}
			else iter++;
		}
	}
	DrawTool *tool = DrawTool::FindTool(selection);
	if (tool != NULL)	tool->OnLButtonUp(pView, nFlags, point);
}

void ClipTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (nFlags & MK_LBUTTON) {
		CRect clipRect(c_down, point);
		pView->setClipRect(clipRect);
	}

	DrawTool::OnMouseMove(pView, nFlags, point);
}

void ClipTool::OnCancel()
{
}

/*
//////////////////////////////////////////////////////////////////////////
*/


CurveTool::CurveTool()
{
	m_drawObj = NULL;
}

CurveTool::CurveTool(DrawShape nDrawShape)
:DrawTool(nDrawShape)
{
	m_drawObj = NULL;
}

CurveTool::~CurveTool()
{
}

void CurveTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool *tool = DrawTool::FindTool(selection);
	if (tool != NULL)	tool->OnLButtonUp(pView, nFlags, point);

	if (m_drawObj == NULL) {
		//确定第一个点
		m_drawObj = new GCurve;
		m_drawObj->m_points[0] = point;
		m_drawObj->m_points[1] = point;
		m_drawObj->m_points[2] = point;
		stage = 1;
		pView->addObject(m_drawObj);
		pView->select(m_drawObj);
	}
	else {
		m_drawObj->m_points[1] = point;
		stage = 2;
	}
}

void CurveTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
}

void CurveTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj != NULL && stage == 2) {
		m_drawObj->m_points[2] = point;
		m_drawObj->m_state = GNone;
		m_drawObj->updateExRect();
		m_drawObj = NULL;
		stage = 0;
	}

}

void CurveTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj != NULL && nFlags & MK_LBUTTON && stage == 2) {
		m_drawObj->m_points[2] = point;
	}
	else if (m_drawObj != NULL && stage == 1) {
		m_drawObj->m_points[1] = point;
		//为了让点保持均匀，设置直线的中点为控制点
		int x = (m_drawObj->m_points[0].x + point.x) / 2;
		int y = (m_drawObj->m_points[0].y + point.y) / 2;
		m_drawObj->m_points[2].SetPoint(x, y);
	}
}

void CurveTool::OnCancel()
{
}



/*
//////////////////////////////////////////////////////////////////////////
*/
MultiCurveTool::MultiCurveTool()
{
	m_drawObj = NULL;
}

MultiCurveTool::MultiCurveTool(DrawShape nDrawShape)
	:DrawTool(nDrawShape)
{
	m_drawObj = NULL;
}

MultiCurveTool::~MultiCurveTool()
{
}

void MultiCurveTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	DrawTool *tool = DrawTool::FindTool(selection);
	if (tool != NULL)	tool->OnLButtonUp(pView, nFlags, point);

	if (m_drawObj == NULL) {
		m_drawObj = new GMultiCurve;
		//m_drawObj->m_ptVec.push_back(point);
		m_drawObj->addPoint(point);
		m_drawObj->m_currentPt = point;
		pView->addObject(m_drawObj);
		pView->select(m_drawObj);
	}
	else {
		m_drawObj->addPoint(point);
		m_drawObj->m_currentPt = point;
		//m_drawObj->m_ptVec.push_back(point);
		if (m_drawObj->m_ptVec.size() == 2) {

		}
	}


}

void MultiCurveTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	m_drawObj->m_state = GSelected;
	m_drawObj->updateExRect();
	m_drawObj = NULL;
}

void MultiCurveTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
}

void MultiCurveTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj != NULL) {
		//m_drawObj->addPoint(point);
		m_drawObj->m_currentPt = point;
	}
}

void MultiCurveTool::OnCancel()
{
}



/*
//////////////////////////////////////////////////////////////////////////
*/
ParallTool::ParallTool(DrawShape nDrawShape)
	:DrawTool(nDrawShape)
{
	m_drawObj = NULL;
}

ParallTool::~ParallTool()
{
}

void ParallTool::OnLButtonDown(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	if (m_drawObj == NULL) {
		m_drawObj = new G3Dgraph();
		pView->addObject(m_drawObj);
	}
	lastPoint = point;
	DrawTool::OnLButtonDown(pView, nFlags, point);
}

void ParallTool::OnLButtonDblClk(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	lastPoint = point;
	DrawTool::OnLButtonDblClk(pView, nFlags, point);
}

void ParallTool::OnLButtonUp(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	lastPoint = point;
	DrawTool::OnLButtonDown(pView, nFlags, point);
}

void ParallTool::OnMouseMove(CMFCOpenGLView * pView, UINT nFlags, const CPoint & point)
{
	
	if (m_drawObj != NULL) {
		CPoint delta = point - lastPoint;

		//按下鼠标右键和x/y/z，控制3D图形绕相应轴旋转
		if (c_xDown && nFlags & MK_RBUTTON) {
			m_drawObj->rotateX(delta);
		}
		else if (c_yDown && nFlags&MK_RBUTTON) {
			m_drawObj->rotateY(delta);
		}
		else if (c_zDown && nFlags&MK_RBUTTON) {
			m_drawObj->rotateZ(delta);
		}
		//按下鼠标左键和x/y/z，控制3D图形绕相应轴平移
		if (c_xDown && nFlags & MK_LBUTTON) {
			m_drawObj->moveX(delta);
		}
		else if (c_yDown && nFlags&MK_LBUTTON) {
			m_drawObj->moveY(delta);
		}
		else if (c_zDown && nFlags&MK_LBUTTON) {
			m_drawObj->moveZ(delta);
		}


		//m_drawObj->moveGraph(delta);
	}

	lastPoint = point;
	DrawTool::OnMouseMove(pView, nFlags, point);
}

void ParallTool::OnMouseWheel(CMFCOpenGLView * pView, UINT nFlags, short zDelta, CPoint & pt)
{
	if (m_drawObj != NULL) {
		if (zDelta > 0)
			m_drawObj->onSize(0.05);
		else if (zDelta < 0)
			m_drawObj->onSize(-0.05);
	}
	
}

void ParallTool::OnCancel()
{
}
