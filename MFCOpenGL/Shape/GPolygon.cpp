#include "stdafx.h"
#include "GPolygon.h"
#include "graph_algorithm.h"
#include "MiniMatrix.h"
#include <vector>

IMPLEMENT_SERIAL(GPolygon, Graph, 1)


GPolygon::GPolygon()
{
	m_state = GDrawing;
}

GPolygon::GPolygon(CRect & rect)
{
	m_exRect = rect;
	m_state = GDrawing;
}


GPolygon::~GPolygon()
{

}

void GPolygon::Serialize(CArchive & ar)
{
	Graph::Serialize(ar);
	if (ar.IsStoring()) {
		//将属性保存到文件
		ar.WriteCount(m_ptVec.size());
		for (int i = 0; i < m_ptVec.size(); i++)
			ar << m_ptVec[i];
	}
	else {
		int size = ar.ReadCount();
		for (int i = 0; i < size; i++) {
			CPoint pt;
			ar >> pt;
			m_ptVec.push_back(pt);
		}
	}
}

void GPolygon::draw()
{
	
	//计算旋转后的点if
	if (m_rad != 0 && m_ptVec.size() != 0) {
		PtVec &pts = this->m_ptVec;
		PtVec::iterator iter = pts.begin();
		CPoint ctr = m_exRect.CenterPoint();
		while (iter != pts.end()) {
			CPoint delta = *iter - ctr;
			int dx = round(transferX(delta.x, delta.y));
			int dy = round(transferY(delta.x, delta.y));
			(*iter).SetPoint(ctr.x + dx, ctr.y + dy);
			iter++;
		}
		m_theta = 0;
		m_rad = 0;
		updateExRect();
	}
	

	if (m_state != GDrawing) {
		glPointSize(1.0);
		glColor3ub(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
		polygonScan(m_ptVec);
	}
	//	this->scanLineSeedFill(m_exRect.CenterPoint.x, m_exRect.CenterPoint.y, m_lineColor, m_fillColor);
	CPoint pre, cur;
	
	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	if (!m_ptVec.empty()) {
		PtVec::iterator iter = m_ptVec.begin();
		cur = *(iter++);
		while (iter != m_ptVec.end()) {
			pre = cur;
			cur = *(iter++);
			drawLineBresenham(pre.x, pre.y, cur.x, cur.y);		
		}
		if(m_state == GDrawing)
			drawLineBresenham(cur.x, cur.y, m_current.x, m_current.y);
		else {
			pre = *m_ptVec.begin();
			drawLineBresenham(pre.x, pre.y, cur.x, cur.y);
		}
	}
}


void GPolygon::updateExRect()
{
	if (this->m_ptVec.size() == 0) {
		m_exRect.SetRect(0, 0, 0, 0);
		return;
	}
	int left, right, top, bottom;
	CPoint point = m_ptVec.front();
	left = right = point.x; top = bottom = point.y;
	for_each(m_ptVec.begin(), m_ptVec.end(),
		[&](CPoint &p) {left = min(left, p.x);
			right = max(right, p.x);
			bottom = min(bottom, p.y);
			top = max(top, p.y);
			}
		);
	m_exRect.SetRect(left, top, right, bottom);
}

void GPolygon::moveGraph(CPoint & point)
{
	for_each(m_ptVec.begin(), m_ptVec.end(),
		[&](CPoint &p) {p += point; }
		);
	m_exRect += point;
}

void GPolygon::moveHandle(GHANDLE nHandle, CPoint & point)
{

	CPoint handlePt = handlePos(nHandle);	//handle point
	int tmp = (int)nHandle + 4;
	if (tmp > 8)	tmp = tmp % 8;
	CPoint fixedPt = handlePos((GHANDLE)tmp);
	double sx, sy;

	switch (nHandle)
	{
	case LT:
	case RB:
	case RT:
	case LB:
		sx = (handlePt.x + point.x - fixedPt.x) * 1.0 / (handlePt.x - fixedPt.x);
		sy = (handlePt.y + point.y - fixedPt.y) * 1.0 / (handlePt.y - fixedPt.y);
		break;
	case L:
	case R:
		sx = (handlePt.x + point.x - fixedPt.x) * 1.0 / (handlePt.x - fixedPt.x);
		sy = 1.0;
		break;
	case T:
	case B:
		sy = (handlePt.y + point.y - fixedPt.y) * 1.0 / (handlePt.y - fixedPt.y);
		sx = 1.0;
		break;
	default:
		break;
	}

	//防止缩小为一点
	if (abs(m_exRect.right - m_exRect.left) < 32 && abs(sx) < 1.0)	sx = 1.0;
	if (abs(m_exRect.top - m_exRect.bottom) < 32 && abs(sy) < 1.0)	sy = 1.0;

	TMatrix<double> T(fixedPt.x, fixedPt.y, 0);
	TMatrix<double> negT(-fixedPt.x, -fixedPt.y, 0);
	SMatrix<double> S(sx, sy, 1.0);
	for (int i = 0; i < m_ptVec.size(); i++) {
		PMatrix<double> P(m_ptVec[i].x, m_ptVec[i].y, 0);
		MiniMatrix<double> M = P * negT * S * T;
		m_ptVec[i].SetPoint(round(M[0][0]), round(M[0][1]));
	}

	updateExRect();
}

int GPolygon::onClip(CRect & clipRect)
{
	PtVec clipPoly;	
	clipPoly.push_back(CPoint(clipRect.left, clipRect.top));
	clipPoly.push_back(CPoint(clipRect.right, clipRect.top));
	clipPoly.push_back(CPoint(clipRect.right, clipRect.bottom));
	clipPoly.push_back(CPoint(clipRect.left, clipRect.bottom));
	Sutherland_Hodgman_clip(m_ptVec, clipPoly);

	updateExRect();

	return m_ptVec.size();
}




