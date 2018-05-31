#include "stdafx.h"
#include "GCircle.h"
#include "graph_algorithm.h"
#include <cmath>

IMPLEMENT_SERIAL(GCircle, Graph, 1)

GCircle::GCircle()
{
}

GCircle::GCircle(CRect & rect)
{
	m_exRect = rect;
	m_rect = rect;
}


GCircle::~GCircle()
{
}

void GCircle::Serialize(CArchive & ar)
{
	Graph::Serialize(ar);
	Graph::Serialize(ar);
	if (ar.IsStoring()) {
		//将属性保存到文件
		ar << m_rect;
	}
	else {
		ar >> m_rect;
	}
}

void GCircle::draw()
{
	CPoint ctr = m_exRect.CenterPoint();

	double r = abs(m_exRect.left - m_exRect.right) / 2;


	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	drawCircleBresenham(ctr.x, ctr.y, r);

	if (abs(m_exRect.top - m_exRect.bottom) > 1) {
		glPointSize(1.0);
		scanLineSeedFill(ctr.x, ctr.y, m_lineColor, m_fillColor);
	}

}

void GCircle::moveHandle(GHANDLE nHandle, CPoint & point)
{
	//Graph::moveHandle(nHandle, point);
	switch (nHandle)
	{
	case LT:
		m_rect.left += point.x;
		m_rect.top += point.y;
		break;
	case T:
		m_rect.top += point.y;
		break;
	case RT:
		m_rect.right += point.x;
		m_rect.top += point.y;
		break;
	case R:
		m_rect.right += point.x;
		break;
	case RB:
		m_rect.right += point.x;
		m_rect.bottom += point.y;
		break;
	case B:
		m_rect.bottom += point.y;
		break;
	case LB:
		m_rect.left += point.x;
		m_rect.bottom += point.y;
		break;
	case L:
		m_rect.left += point.x;
		break;
	default:
		break;
	}

	m_exRect = m_rect;
	int height = abs(m_rect.top- m_rect.bottom);
	int width = abs(m_rect.right - m_rect.left);
	int delta = height - width;
	if (delta < 0) {
		if (m_rect.left > m_rect.right)	m_exRect.left += delta;
		else m_exRect.left -= delta;
	}
	else {
		if (m_rect.top > m_rect.bottom)	m_exRect.top -= delta;
		else m_exRect.top += delta;
	}
}

