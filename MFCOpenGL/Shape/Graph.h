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

	//绘制
	virtual void draw();
	virtual void drawHandle();
	virtual void drawExRect();
	//virtual void drawRotateHandle();
	//区域填充
	int fillRow(int leftX, int rightX, int y);
	void boundaryFill4Init(CPoint &seed);
	void boundaryFill8Init(CPoint &seed);

	void setlineColor(byte r, byte g, byte b);
	void setfillColor(byte r, byte g, byte b);

	CPoint handlePos(GHANDLE nHandle);
	CRect handleRect(GHANDLE nHandle);
	int hitTest(const CPoint &point);

	virtual void moveGraph(CPoint &point);	//平移
	virtual void moveHandle(GHANDLE nHandle, CPoint &point);	//缩放
	virtual void rotate(double theta);				//右旋theta度
	double transferX(int x, int y) { return (x*cos(m_rad) + y*sin(m_rad)); }
	double transferY(int x, int y) { return (-x*sin(m_rad) + y*cos(m_rad)); }

	//裁剪
	virtual int onClip(CRect &clipRect);

public:
	CRect m_exRect;		//一个图形的外接矩阵
	COLORREF m_lineColor;	//图形颜色
	COLORREF m_fillColor;	//填充颜色
	double m_theta;		//旋转参数
	double m_rad;

	GSTATE m_state;		//图形状态
};

#endif // !GRAPH_H