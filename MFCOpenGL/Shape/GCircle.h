#pragma once

#include "Graph.h"

class GCircle: public Graph
{
public:
	DECLARE_SERIAL(GCircle);
	GCircle();
	GCircle(CRect &rect);
	~GCircle();

	virtual void Serialize(CArchive &ar);

	virtual void draw();
	void moveHandle(GHANDLE nHandle, CPoint &point);

private:
	CRect m_rect;			
};

