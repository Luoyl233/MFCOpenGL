#include "stdafx.h"
#include "GMultiCurve.h"
#include "graph_algorithm.h"
#include "MiniMatrix.h"

IMPLEMENT_SERIAL(GMultiCurve, Graph, 1)


GMultiCurve::GMultiCurve()
{
	m_state = GDrawing;
}


GMultiCurve::~GMultiCurve()
{
}

void GMultiCurve::Serialize(CArchive & ar)
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

void GMultiCurve::draw()
{
	if (m_state == GDrawing) {
		m_ptVec.push_back(m_currentPt);
	}

	if (m_rad != 0 && m_ptVec.size() != 0) {
		std::vector<CPoint> &pts = this->m_ptVec;
		std::vector<CPoint>::iterator iter = pts.begin();
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


	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	Bezir_n(m_ptVec);

	if (m_state == GDrawing) {
		m_ptVec.pop_back();
	}

}

void GMultiCurve::moveGraph(CPoint & point)
{
	Graph::moveGraph(point);

	for (int i = 0; i < m_ptVec.size(); i++) {
		m_ptVec[i] += point;
	}
}

void GMultiCurve::moveHandle(GHANDLE nHandle, CPoint & point)
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

void GMultiCurve::updateExRect()
{
	if (this->m_ptVec.size() == 0) {
		m_exRect.SetRect(0, 0, 0, 0);
		return;
	}
	int left, right, top, bottom;
	CPoint point = m_ptVec.front();
	left = right = point.x; top = bottom = point.y;
	for (int i = 0; i < m_ptVec.size(); i++) {
		left = (m_ptVec[i].x < left) ? m_ptVec[i].x : left;
		right = (m_ptVec[i].x > right) ? m_ptVec[i].x : right;
		top = (m_ptVec[i].y > top) ? m_ptVec[i].y : top;
		bottom = (m_ptVec[i].y < bottom) ? m_ptVec[i].y : bottom;
	}

	m_exRect.SetRect(left, top, right, bottom);
}

void GMultiCurve::addPoint(const CPoint & point)
{
	if (m_ptVec.size() == 0 || point != m_ptVec.back()) {
		m_ptVec.push_back(point);
	}
}
