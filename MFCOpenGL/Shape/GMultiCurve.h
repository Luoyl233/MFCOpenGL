#pragma once
#include "Graph.h"
#include <vector>

class GMultiCurve: public Graph
{
public:
	DECLARE_SERIAL(GMultiCurve);

	GMultiCurve();
	~GMultiCurve();

	virtual void Serialize(CArchive &ar);
	virtual void draw();
	virtual void moveGraph(CPoint &point);	//ƽ��
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//����
	void updateExRect();

	void addPoint(const CPoint &point);

public:
	std::vector<CPoint> m_ptVec;
	CPoint m_currentPt;
};

