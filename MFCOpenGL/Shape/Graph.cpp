#include "stdafx.h"
#include "Graph.h"
#include <stack>

IMPLEMENT_SERIAL(Graph, CObject, 1)

Graph::Graph()
{
	m_exRect.SetRect(0, 0, 0, 0);
	m_lineColor = RGB(0, 0, 0);
	m_fillColor = RGB(0, 255, 255);
	m_state = GNone;
	m_theta = 0.0;
	m_rad = m_theta * Pi / 180 ;
}

Graph::~Graph()
{
}

void Graph::Serialize(CArchive & ar)
{
	CObject::Serialize(ar);
	if (ar.IsStoring()) {
		//将属性保存到文件
		ar << m_exRect;
		ar << m_lineColor;
		ar << m_fillColor;
		ar << m_theta;
		ar << m_rad;
		ar << m_state;
	}
	else {
		//将属性读取到程序
		ar >> m_exRect;
		ar >> m_lineColor;
		ar >> m_fillColor;
		ar >> m_theta;
		ar >> m_rad;
		int tmp;
		ar >> tmp;
		m_state = (GSTATE)tmp;
	}
}


void Graph::draw()
{
	if (m_state == GSelected) {
		drawExRect();
		drawHandle();
	}
}

void Graph::drawHandle()
{
	CPoint point;
	glColor3i(1.0, 1.0, 1.0);
	glPointSize(4.0);
	glBegin(GL_POINTS);

	for (int i = LT; i <= L; i++) {
		point = handlePos((GHANDLE)i);
		glVertex2i(point.x, point.y);
	}
	glEnd();
	/*
	CRect  rect = m_exRect;
	rect.NormalizeRect();
	CPoint topMid((rect.left + rect.right)/2, rect.top);
	glColor3i(1.0, 1.0, 1.0);
	glPointSize(1.0);
	glBegin(GL_LINE);
	glVertex2i(topMid.x, topMid.y);
	glVertex2i(topMid.x, topMid.y+20);
	glEnd();
	//glVertex2i(point.x, point.y);
	*/

}

void Graph::drawExRect()
{
	glColor3f(0.0, 0.0, 1.0);
	glLineWidth(1.0);
	glLineStipple(1, 0x0F0F);
	glEnable(GL_LINE_STIPPLE);

	glBegin(GL_LINES);
	glVertex2i(m_exRect.left, m_exRect.top);
	glVertex2i(m_exRect.left, m_exRect.bottom);
	glVertex2i(m_exRect.right, m_exRect.bottom);
	glVertex2i(m_exRect.right, m_exRect.top);
	glVertex2i(m_exRect.left, m_exRect.top);
	glVertex2i(m_exRect.right, m_exRect.top);
	glVertex2i(m_exRect.left, m_exRect.bottom);
	glVertex2i(m_exRect.right, m_exRect.bottom);
	glEnd();
	glDisable(GL_LINE_STIPPLE);
}

int Graph::fillRow(int leftX, int rightX, int y)
{
	int i;
	glPointSize(1.0);
	glColor3ub(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
	glBegin(GL_POINTS);
	for (i = leftX; i <= rightX; i++)
		glVertex2i(i, y);
	glEnd();
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	
	return i - leftX;
}

void Graph::boundaryFill4Init(CPoint &seed)
{
	glPointSize(1.0);
	glColor3ub(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
	setColor(m_fillColor);
	glBegin(GL_POINTS);
	boundaryFill4(seed.x + WIDTH / 2, seed.y + HEIGHT / 2, m_lineColor, m_fillColor);
	glEnd();
}

void Graph::boundaryFill8Init(CPoint &seed)
{
	glPointSize(1.0);
	glColor3ub(GetRValue(m_fillColor), GetGValue(m_fillColor), GetBValue(m_fillColor));
	glBegin(GL_POINTS);
	boundaryFill8(seed.x+WIDTH/2, seed.y+HEIGHT/2, m_lineColor, m_fillColor);
	glEnd();
}





void Graph::setlineColor(byte r, byte g, byte b)
{
	m_lineColor = RGB(r, g, b);
}

void Graph::setfillColor(byte r, byte g, byte b)
{
	m_fillColor = RGB(r, g, b);
}

CPoint Graph::handlePos(GHANDLE nHandle)
{
	CPoint point(0, 0);
	switch (nHandle)
	{
	case LT:
		point.SetPoint(m_exRect.left, m_exRect.top);
		break;
	case T:
		point.SetPoint((m_exRect.left + m_exRect.right ) / 2, m_exRect.top);
		break;
	case RT:
		point.SetPoint(m_exRect.right, m_exRect.top);
		break;
	case R:
		point.SetPoint(m_exRect.right, (m_exRect.bottom + m_exRect.top) / 2);
		break;
	case RB:
		point.SetPoint(m_exRect.right, m_exRect.bottom);
		break;
	case B:
		point.SetPoint((m_exRect.left + m_exRect.right) / 2, m_exRect.bottom);
		break;
	case LB:
		point.SetPoint(m_exRect.left, m_exRect.bottom);
		break;
	case L:
		point.SetPoint(m_exRect.left, (m_exRect.bottom + m_exRect.top) / 2);
		break;
	default:
		ASSERT(false);
		break;
	}
	return point;
}

CRect Graph::handleRect(GHANDLE nHandle)
{
	CPoint point = handlePos(nHandle);

	return CRect(point.x - 3, point.y + 3, point.x + 3, point.y - 3);
}

int Graph::hitTest(const CPoint & point)
{
	CRect rect;

	for (int i = LT; i <= L; i++) {
		rect = handleRect((GHANDLE)i);
		if (point.x >= rect.left && point.x <= rect.right && point.y >= rect.bottom && point.y <= rect.top)
			return i;
	}

	rect = m_exRect;
	rect.NormalizeRect();
	//if (point.x >= rect.left && point.x <= rect.right && point.y >= rect.bottom && point.y <= rect.top)
	if(rect.PtInRect(point))
		return 0;

	return -1;
}

void Graph::moveGraph(CPoint & point)
{
	m_exRect += point;
}

void Graph::moveHandle(GHANDLE nHandle, CPoint & point)
{
	switch (nHandle)
	{
	case LT:
		m_exRect.left += point.x;
		m_exRect.top += point.y;
		break;
	case T:
		m_exRect.top += point.y;
		break;
	case RT:
		m_exRect.right += point.x;
		m_exRect.top += point.y;
		break;
	case R:
		m_exRect.right += point.x;
		break;
	case RB:
		m_exRect.right += point.x;
		m_exRect.bottom += point.y;
		break;
	case B:
		m_exRect.bottom += point.y;
		break;
	case LB:
		m_exRect.left += point.x;
		m_exRect.bottom += point.y;
		break;
	case L:
		m_exRect.left += point.x;
		break;
	default:
		break;
	}
}

void Graph::rotate(double theta)
{
	m_theta += theta;
	m_rad = m_theta * Pi / 180.0;
}

int Graph::onClip(CRect & clipRect)
{
	return true;
}



