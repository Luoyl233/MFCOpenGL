#include "stdafx.h"
#include "GLine.h"
#include "graph_algorithm.h"

IMPLEMENT_SERIAL(GLine, Graph, 1)

GLine::GLine()
{
}

GLine::GLine(CRect & rect)
{
	m_exRect = rect;
}

GLine::GLine(GLint cx, GLint cy)
{
}


GLine::~GLine()
{
}

int GLine::onClip(CRect & clipRect)
{
	CRect normalClipRect = clipRect;
	normalClipRect.NormalizeRect();
	int tmp = normalClipRect.bottom;
	normalClipRect.bottom = normalClipRect.top;
	normalClipRect.top = tmp;
	if (this->Liang_BarskyClip(normalClipRect))
		return true;

	return false;
}

int GLine::clipTest(float pk, float qk, float & u1, float & u2)
{
	bool flag = true;
	float r;


	if (pk < 0.0) {
		//pk < 0.0, 参数r更新u1
		r = qk / pk;
		if (r > u2)	flag = false;
		else if (r > u1) u1 = r;
	}
	else if (pk > 0.0) {
		//pk > 0.0, 参数r更新u2
		r = qk / pk;
		if (r < u1) flag = false;
		else if (r < u2)	u2 = r;
	}
	else if(qk < 0.0){
		//pk = 0.0, 并且qk < 0.0, 则说明该线段平行于边界且位于边界之外
		flag = false;
	}

	return flag;
}


int GLine::Liang_BarskyClip(const CRect & clipRect)
{
	bool exist = false;
	float u1, u2, delta_x, delta_y;
	//m_exRect.NormalizeRect();
	u1 = 0.0; u2 = 1.0; delta_x = this->m_exRect.right - m_exRect.left;
	if(clipTest(-delta_x, this->m_exRect.left-clipRect.left, u1, u2))
		if (clipTest(delta_x, clipRect.right - m_exRect.left, u1, u2)) {
			delta_y = m_exRect.top - m_exRect.bottom;
			if(clipTest(-delta_y, m_exRect.bottom-clipRect.bottom, u1, u2))
				if (clipTest(delta_y, clipRect.top-m_exRect.bottom, u1, u2)) {
					exist = true;
					if (u2 < 1.0) {
						m_exRect.right = m_exRect.left + u2 *delta_x;
						m_exRect.top = m_exRect.bottom + u2 * delta_y;
					}
					if (u1 > 0.0) {
						m_exRect.left = m_exRect.left + u1 * delta_x;
						m_exRect.bottom = m_exRect.bottom + u1 * delta_y;
					}
				}
		}

	return exist;
}

void GLine::draw()
{
	if (m_theta != 0) {
		CPoint ctr = m_exRect.CenterPoint();
		CPoint delta = m_exRect.TopLeft() - ctr;
		int dx = round(transferX(delta.x, delta.y));
		int dy = round(transferY(delta.x, delta.y));
		delta = m_exRect.BottomRight() - ctr;
		int dx2 = round(transferX(delta.x, delta.y));
		int dy2 = round(transferY(delta.x, delta.y));
		m_exRect.SetRect(dx, dy, dx2, dy2);

		m_theta = 0;
		m_rad = 0;
	}


	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
	setColor(m_lineColor);
	drawLineBresenham(m_exRect.left, m_exRect.top, m_exRect.right, m_exRect.bottom);

}
