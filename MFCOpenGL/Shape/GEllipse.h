#pragma once

#include "Graph.h"

class GEllipse: public Graph
{
public:
	DECLARE_SERIAL(GEllipse);

	GEllipse();
	GEllipse(CRect &rect);
	~GEllipse();

	virtual void draw();


private:
};

