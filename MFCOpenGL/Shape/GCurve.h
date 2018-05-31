#pragma once
#include "Graph.h"

class GCurve: public Graph
{
public:
	DECLARE_SERIAL(GCurve);

	GCurve();
	~GCurve();

	virtual void draw();
	virtual void moveGraph(CPoint &point);	//Æ½ÒÆ
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//Ëõ·Å
	virtual void Serialize(CArchive &ar);

	void updateExRect();

public:
	CPoint m_points[3];		//
};

