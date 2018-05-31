#pragma once

#include "Graph.h"
#include "Point3D.h"

class G3Dgraph:public Graph
{
public:
	DECLARE_SERIAL(G3Dgraph);

	G3Dgraph();
	~G3Dgraph();

	virtual void Serialize(CArchive &ar);
	virtual void draw();


	void rotateX(CPoint &point);
	void rotateY(CPoint &point);
	void rotateZ(CPoint &point);
	void moveX(CPoint &point);
	void moveY(CPoint &point);
	void moveZ(CPoint &point);
	void onSize(const double &s);

public:
	double m_vertexList[8][3];

	int length;
};

