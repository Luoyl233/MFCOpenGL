#pragma once
#include "Graph.h"


class GLine: public Graph
{
public:
	DECLARE_SERIAL(GLine);
	GLine();
	GLine(CRect &rect);
	GLine(GLint cx, GLint cy);
	~GLine();
	void draw();

	//²Ã¼ô
	virtual int onClip(CRect &clipRect);
	int clipTest(float pk, float qk, float&u1, float &u2);
	int Liang_BarskyClip(const CRect &clipRect);	//liang-Barsky clip algrithm

private:
};

