#include "stdafx.h"
#include "GEllipse.h"
#include "graph_algorithm.h"

IMPLEMENT_SERIAL(GEllipse, Graph, 1)

GEllipse::GEllipse()
{
}

GEllipse::GEllipse(CRect & rect)
{
	m_exRect = rect;
}


GEllipse::~GEllipse()
{
}


void GEllipse::draw()
{
	int a = abs(m_exRect.left - m_exRect.right) / 2;
	int b = abs(m_exRect.top - m_exRect.bottom) / 2;
	CPoint ctr = m_exRect.CenterPoint();

	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	if (a > 0 && b > 0) {
		drawEllipseBresenham(ctr.x, ctr.y, a, b);
		//this->boundaryFill4Init(ctr);
		if(m_theta == 0)
			scanLineSeedFill(ctr.x, ctr.y, m_lineColor, m_fillColor);
	}

}
