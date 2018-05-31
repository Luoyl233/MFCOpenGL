#include "stdafx.h"
#include "GCurve.h"
#include "graph_algorithm.h"
#include "MiniMatrix.h"

IMPLEMENT_SERIAL(GCurve, Graph, 1)

GCurve::GCurve()
{
}


GCurve::~GCurve()
{
}

void GCurve::draw()
{

	if (m_theta != 0) {
		CPoint ctr = m_exRect.CenterPoint();
		for (int i = 0; i < 3; i++) {
			CPoint delta = m_points[i] - ctr;
			int dx = round(transferX(delta.x, delta.y));
			int dy = round(transferY(delta.x, delta.y));
			m_points[i].SetPoint(dx, dy);
		}
		m_theta = 0;
		m_rad = 0;
		updateExRect();
	}


	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	Bezir_2(m_points[0], m_points[2], m_points[1]);
}

void GCurve::moveGraph(CPoint & point)
{
	Graph::moveGraph(point);

	m_points[0] += point;
	m_points[1] += point;
	m_points[2] += point;
}

void GCurve::moveHandle(GHANDLE nHandle, CPoint & point)
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
	for (int i = 0; i < 3; i++) {
		PMatrix<double> P(m_points[i].x, m_points[i].y, 0);
		MiniMatrix<double> M = P * negT * S * T;
		m_points[i].SetPoint(round(M[0][0]), round(M[0][1]));
	}
	

	updateExRect();
}

void GCurve::Serialize(CArchive & ar)
{
	Graph::Serialize(ar);
	if (ar.IsStoring()) {
		//将属性保存到文件
		ar << m_points[0];
		ar << m_points[1];
		ar << m_points[2];
	}
	else {
		ar >> m_points[0];
		ar >> m_points[1];
		ar >> m_points[2];
	}
}

void GCurve::updateExRect()
{
	int left, right, top, bottom;
	left = right = m_points[0].x;
	top = bottom = m_points[0].y;

	for (int i = 1; i < 3; i++) {
		left = (m_points[i].x < left) ? m_points[i].x : left;
		right = (m_points[i].x > right) ? m_points[i].x : right;
		top = (m_points[i].y > top) ? m_points[i].y : top;
		bottom = (m_points[i].y < bottom) ? m_points[i].y : bottom;
	}
	
	m_exRect.SetRect(left, top, right, bottom);
}
