#pragma once

#include "Graph.h"
#include <vector>
#include <algorithm>

typedef std::vector<CPoint> PtVec;

class GPolygon: public Graph
{
public:
	DECLARE_SERIAL(GPolygon);

	GPolygon();
	GPolygon(CRect &rect);
	~GPolygon();

	virtual void Serialize(CArchive &ar);
	virtual void draw();
	virtual void moveGraph(CPoint &point);	//ƽ��
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//����
	virtual int onClip(CRect &clipRect);

	void updateExRect();

public:
	PtVec m_ptVec;
	CPoint m_current;		//��굱ǰλ��
};

