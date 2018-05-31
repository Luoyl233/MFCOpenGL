#pragma once

#include "Graph.h"

class GRect: public Graph
{
public:
	DECLARE_SERIAL(GRect);

	GRect();
	GRect(CRect &rect);
	~GRect();

	virtual void draw();
};

