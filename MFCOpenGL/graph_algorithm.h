

#ifndef GRAPH_ALGORITHM_H
#define GRAPH_ALGORITHM_H
#include "stdafx.h"
#include <vector>


const int WIDTH = 1000;
const int HEIGHT = 1000;


//Brensenham������ֱ��
void drawLineBresenham(int x1, int y1, int x2, int y2);
//�е㻭Բ��
void drawCircleBresenham(int ctrx, int ctry, float r);
//�е㻭��Բ��
void drawEllipseBresenham(int ctrx, int ctry, int a, int b);
//����ת����Բ
void drawRotateEllipse(int ctrx, int ctry, int a, int b, double theta);
//�������
void drawPolygon(std::vector<CPoint> &ptVec);
//�����ɨ������㷨
void polygonScan(std::vector<CPoint> &ptVec);

//����βü�
int Sutherland_Hodgman_clip(std::vector<CPoint> &srcPoly, std::vector<CPoint> &clipPoly);


//ɨ���������
void boundaryFill4(int x, int y, COLORREF boundaryColor, COLORREF fillColor);	//4�߽����
void boundaryFill8(int x, int y, COLORREF boundaryColor, COLORREF fillColor);	//8�߽����
void scanLineSeedFill(int x, int y, COLORREF boundaryColor, COLORREF fillColor);//ɨ���߽߱����


//Bezir-2�����߻���
void Bezir_2(CPoint &start, CPoint &end, CPoint &control);
//Bezir-3�����߻���
void Bezir_3(CPoint &start, CPoint &end, CPoint &ctrl1, CPoint &ctrl2);
//Bezir-n����������߻���
void Bezir_n(std::vector<CPoint> &ptVec);





#endif