#pragma once

#include <vector>
#include "Shape\Graph.h"
#include "Shape\GRect.h"
#include "Shape\GPolygon.h"
#include "Shape\GFill.h"
#include "Shape\GCurve.h"
#include "Shape\GMultiCurve.h"
#include "Shape\G3Dgraph.h"
#include "MFCOpenGLView.h"

enum DrawShape
{
	selection,
	line,
	rect,
	circle,
	ellipse,
	poly,
	fill,
	curve,
	multiCurve,
	parallelepiped,
	clip,
	shapeNum
};

enum SelectMode {
	SNone,
	SMove,
	SSize
};


class DrawTool;
class CMFCOpenGLView;
typedef std::vector<DrawTool *> ToolVec;

class DrawTool
{
public:
	DrawTool();
	DrawTool(DrawShape nDrawShape);
	~DrawTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseWheel(CMFCOpenGLView* pView, UINT nFlags, short zDelta, CPoint &pt);
	virtual void OnKeyDown(CMFCOpenGLView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnKeyUp(CMFCOpenGLView* pView, UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual void OnEditProperties(CMFCOpenGLView* pView);
	virtual void OnCancel();

	// Attributes
	DrawShape m_drawShape;

	static DrawTool* FindTool(DrawShape drawShape);
	static ToolVec c_tools;
	static CPoint c_down;
	static UINT c_nDownFlags;
	static CPoint c_last;
	static DrawShape c_drawShape;

	static bool c_xDown;
	static bool c_yDown;
	static bool c_zDown;


protected:

private:

};

class RectTool : public DrawTool
{
public:
	RectTool();
	RectTool(DrawShape nDrawShape);
	~RectTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CMFCOpenGLView* pView);
	virtual void OnCancel();
};

class SelectTool : public DrawTool
{
public:
	SelectTool(DrawShape nDrawShape);
	~SelectTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnEditProperties(CMFCOpenGLView* pView);
	virtual void OnCancel();


};

class PolyTool: public DrawTool
{
public:
	PolyTool();
	PolyTool(DrawShape nDrawShape);
	~PolyTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	GPolygon *m_drawObj;
};

class ClipTool: public DrawTool
{
public:
	ClipTool();
	ClipTool(DrawShape nDrawShape);
	~ClipTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

};

class CurveTool : public DrawTool
{
public:
	CurveTool();
	CurveTool(DrawShape nDrawShape);
	~CurveTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	GCurve *m_drawObj;
	int stage;
};


class MultiCurveTool : public DrawTool
{
public:
	MultiCurveTool();
	MultiCurveTool(DrawShape nDrawShape);
	~MultiCurveTool();

	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnCancel();

	GMultiCurve *m_drawObj;
	int stage;
};


class ParallTool:public DrawTool
{
public:
	ParallTool(DrawShape nDrawShape);
	~ParallTool();


	// Overridables
	virtual void OnLButtonDown(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonDblClk(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnLButtonUp(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseMove(CMFCOpenGLView* pView, UINT nFlags, const CPoint& point);
	virtual void OnMouseWheel(CMFCOpenGLView* pView, UINT nFlags, short zDelta, CPoint &pt);
	virtual void OnCancel();

private:
	G3Dgraph *m_drawObj;
};
