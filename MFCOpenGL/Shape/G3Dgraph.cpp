#include "stdafx.h"
#include "G3Dgraph.h"
#include "MiniMatrix.h"
#include "graph_algorithm.h"
#include <vector>

IMPLEMENT_SERIAL(G3Dgraph, Graph, 1)

static double c_vertexList[8][3] = {
	-50, -50, -50,
	50, -50, -50,
	50, 50, -50,
	-50, 50, -50,
	-50, -50, 50,
	50, -50, 50,
	50, 50, 50,
	-50, 50, 50
};
static int c_indexList[6][4] = {
	3, 2, 1, 0,
	0, 4, 7, 3,
	0, 1, 5, 4,
	4, 5, 6, 7,
	7, 6, 2, 3,
	1, 2, 6, 5
};

COLORREF faceColor[6] = {
	RGB(255, 0, 0),
	RGB(255, 255, 0),
	RGB(0, 255, 0),
	RGB(0, 255, 255),
	RGB(255, 255, 255),
	RGB(255, 0, 255)
};



G3Dgraph::G3Dgraph()
{
	length = 100;

	memcpy(m_vertexList, c_vertexList, 8 * 3 * sizeof(double));
}


G3Dgraph::~G3Dgraph()
{
}

void G3Dgraph::Serialize(CArchive & ar)
{
	Graph::Serialize(ar);
	if (ar.IsStoring()) {
		//将属性保存到文件
		for (int i = 0; i < 8; i++) {
			ar << m_vertexList[i][0];
			ar << m_vertexList[i][1];
			ar << m_vertexList[i][2];
		}
	}
	else {
		for (int i = 0; i < 8; i++) {
			ar >> m_vertexList[i][0];
			ar >> m_vertexList[i][1];
			ar >> m_vertexList[i][2];
		}
	}
}

void G3Dgraph::draw()
{
	glFrontFace(GL_CCW);
	glCullFace(GL_BACK);
	glEnable(GL_CULL_FACE);
	
	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//glPolygonMode(GL_FRONT_AND_BACK, GL);
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	glPointSize(1.0);
	glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));

	//
	//gluLookAt(5.0, 5.0, 5.0, 2.5, 2.5, 0.0, 0.0, 1.0, 0.0);

	//gluLookAt(0.0, 0.0, 20.0, 0.0, 0.0, 0.0, 1.0, 1.0, 1.0);
	//glutSolidSphere(100.0, 20, 16);
	//glScalef(1.0, 2.0, 1.0);
	//glutWireCube(100.0);

	
	/*
	LMatrix<double> L(0.5, 45.0 * Pi / 180.0);		//斜二投影
	std::vector<CPoint> vecPt;
	for (int i = 0; i < 6; i++) {
		int *index = c_indexList[i];
		for (int j = 0; j < 4; j++) {
			int k = index[j];
			PMatrix<double> P(m_vertexList[k][0], m_vertexList[k][1], m_vertexList[k][2]);
			MiniMatrix<double> M = P * L;

			int x = M[0][0];
			int y = M[0][1];
			vecPt.push_back(CPoint(x, y));
		}
		glPointSize(1.0);
		glColor3ub(GetRValue(faceColor[i]), GetGValue(faceColor[i]), GetBValue(faceColor[i]));
		polygonScan(vecPt);
		glColor3ub(GetRValue(m_lineColor), GetGValue(m_lineColor), GetBValue(m_lineColor));
		drawPolygon(vecPt);
		vecPt.clear();
	}
	*/





	
	glBegin(GL_QUADS);
	for (int i = 0; i < 6; i++) {
		for (int j = 0; j < 4; j++) {
			//glColor3ub(GetRValue(faceColor[i]), GetGValue(faceColor[i]), GetBValue(faceColor[i]));
			//glVertex3iv(m_vertexList[m_indexList[i][j]]);
			glVertex3dv(m_vertexList[c_indexList[i][j]]);
		}
	}
	glEnd();
	
}


void G3Dgraph::rotateX(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	double theta = offset * Pi / length;

	RxMatrix<double> R(theta);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * R;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::rotateY(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	double theta = offset * Pi / length;

	RyMatrix<double> R(theta);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * R;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::rotateZ(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	double theta = offset * Pi / length;

	RzMatrix<double> R(theta);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * R;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::moveX(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	TMatrix<double> T(offset, 0, 0);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * T;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::moveY(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	TMatrix<double> T(0, offset, 0);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * T;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::moveZ(CPoint & point)
{
	double offset = 0.0;
	if (abs(point.x) > abs(point.y)) {
		offset = point.x;
	}
	else
		offset = point.y;
	TMatrix<double> T(0, 0, offset);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		MiniMatrix<double> M = P * T;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}

void G3Dgraph::onSize(const double &s)
{
	SMatrix<double> S(1.0 + s, 1.0 + s, 1.0 + s);
	for (int i = 0; i < 8; i++) {
		double *pVertex = m_vertexList[i];
		PMatrix<double> P(m_vertexList[i][0], m_vertexList[i][1], m_vertexList[i][2]);
		//MiniMatrix<double> M = P * T;

		//MiniMatrix<double> M = P * (-T) * S * T;
		MiniMatrix<double> M = P * S;

		pVertex[0] = (M[0][0]);
		pVertex[1] = (M[0][1]);
		pVertex[2] = (M[0][2]);
	}
}
