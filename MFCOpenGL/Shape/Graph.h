//#pragma once
#ifndef GRAPH_H
#define GRAPH_H

#include "pixels.h"
#include "graph_algorithm.h"


const double Pi = 3.1415926;

enum GSTATE {
	GNone,
	GDrawing,
	GSelected
};

enum GHANDLE {
	LT = 1,		//top left
	T,
	RT,
	R,
	RB,
	B,
	LB,
	L
};



class Graph:public CObject
{
public:
	DECLARE_SERIAL(Graph);

	Graph();
	virtual ~Graph();
	virtual void Serialize(CArchive &ar);

	//����
	virtual void draw();
	virtual void drawHandle();
	virtual void drawExRect();
	//virtual void drawRotateHandle();
	//�������
	int fillRow(int leftX, int rightX, int y);
	void boundaryFill4Init(CPoint &seed);
	void boundaryFill8Init(CPoint &seed);

	void setlineColor(byte r, byte g, byte b);
	void setfillColor(byte r, byte g, byte b);

	CPoint handlePos(GHANDLE nHandle);
	CRect handleRect(GHANDLE nHandle);
	int hitTest(const CPoint &point);

	virtual void moveGraph(CPoint &point);	//ƽ��
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//����
	virtual void rotate(double theta);				//����theta��
	double transferX(int x, int y) { return (x*cos(m_rad) + y*sin(m_rad)); }
	double transferY(int x, int y) { return (-x*sin(m_rad) + y*cos(m_rad)); }

	//�ü�
	virtual int onClip(CRect &clipRect);

public:
	CRect m_exRect;		//һ��ͼ�ε���Ӿ���
	COLORREF m_lineColor;	//ͼ����ɫ
	COLORREF m_fillColor;	//�����ɫ
	double m_theta;		//��ת����
	double m_rad;

	GSTATE m_state;		//ͼ��״̬
};

#endif // !GRAPH_H