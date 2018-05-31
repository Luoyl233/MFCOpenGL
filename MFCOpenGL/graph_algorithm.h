

#ifndef GRAPH_ALGORITHM_H
#define GRAPH_ALGORITHM_H
#include "stdafx.h"
#include <vector>


const int WIDTH = 1000;
const int HEIGHT = 1000;


//Brensenham法绘制直线
void drawLineBresenham(int x1, int y1, int x2, int y2);
//中点画圆法
void drawCircleBresenham(int ctrx, int ctry, float r);
//中点画椭圆法
void drawEllipseBresenham(int ctrx, int ctry, int a, int b);
//画旋转的椭圆
void drawRotateEllipse(int ctrx, int ctry, int a, int b, double theta);
//画多边形
void drawPolygon(std::vector<CPoint> &ptVec);
//多边形扫描填充算法
void polygonScan(std::vector<CPoint> &ptVec);

//多边形裁剪
int Sutherland_Hodgman_clip(std::vector<CPoint> &srcPoly, std::vector<CPoint> &clipPoly);


//扫描区域填充
void boundaryFill4(int x, int y, COLORREF boundaryColor, COLORREF fillColor);	//4边界填充
void boundaryFill8(int x, int y, COLORREF boundaryColor, COLORREF fillColor);	//8边界填充
void scanLineSeedFill(int x, int y, COLORREF boundaryColor, COLORREF fillColor);//扫描线边界填充


//Bezir-2阶曲线绘制
void Bezir_2(CPoint &start, CPoint &end, CPoint &control);
//Bezir-3阶曲线绘制
void Bezir_3(CPoint &start, CPoint &end, CPoint &ctrl1, CPoint &ctrl2);
//Bezir-n个顶点的曲线绘制
void Bezir_n(std::vector<CPoint> &ptVec);





#endif