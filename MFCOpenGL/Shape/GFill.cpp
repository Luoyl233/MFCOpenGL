#include "stdafx.h"
#include "GFill.h"


IMPLEMENT_SERIAL(GFill, Graph, 1)

GFill::GFill()
{
}

GFill::GFill(CRect & rect)
{
	m_exRect = rect;
}


GFill::~GFill()
{
}

void GFill::draw()
{
	CPoint ctr = m_exRect.CenterPoint();

	scanLineSeedFill(ctr.x, ctr.y, m_lineColor, m_fillColor);
}
