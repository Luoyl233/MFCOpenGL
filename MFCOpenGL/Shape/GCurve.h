#pragma once
#include "Graph.h"

class GCurve: public Graph
{
public:
	DECLARE_SERIAL(GCurve);

	GCurve();
	~GCurve();

	virtual void draw();
	virtual void moveGraph(CPoint &point);	//ƽ��
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//����
	virtual void Serialize(CArchive &ar);

	void updateExRect();

public:
	CPoint m_points[3];		//
};

