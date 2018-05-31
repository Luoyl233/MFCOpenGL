#pragma once

#include "Graph.h"

class GFill: public Graph
{
public:
	DECLARE_SERIAL(GFill);

	GFill();
	GFill(CRect &rect);
	~GFill();

	virtual void draw();
};

