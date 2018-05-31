#include "stdafx.h"
#include "GRect.h"
#include "graph_algorithm.h"


IMPLEMENT_SERIAL(GRect, Graph, 1)

GRect::GRect()
{
}

GRect::GRect(CRect & rect)
{
	m_exRect = rect;
}


GRect::~GRect()
{
}

void GRect::draw()
{
	std::vector<CPoint> ptVec;
	ptVec.push_back(CPoint(m_exRect.left, m_exRect.top));
	ptVec.push_back(CPoint(m_exRect.right, m_exRect.top));
	ptVec.push_back(CPoint(m_exRect.right, m_exRect.bottom));
	ptVec.push_back(CPoint(m_exRect.left, m_exRect.bottom));

	glPointSize(1.0);
	setColor(m_lineColor);
	glColor3ub(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
	polygonScan(ptVec);


	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	drawPolygon(ptVec);
}
