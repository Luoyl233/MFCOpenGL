#include "stdafx.h"
#include "graph_algorithm.h"
#include "Shape\pixels.h"

#include <vector>
#include <algorithm>
#include <stack>


void drawLineBresenham(int x1, int y1, int x2, int y2)
{
	GLint delta_x, delta_y, p_k, x_k, y_k;
	GLint step_x, step_y;
	GLfloat m;	//y = mx + b

	delta_x = abs(x1 - x2);
	delta_y = abs(y1 - y2);

	//选取第一个点为起点
	x_k = x1;
	y_k = y1;


	/*
	*step 1. 判断沿x和y方向以单位步长变化
	*x1 > x2，x方向肯定是增长的
	*x2 < x2, x方向肯定是减少的
	*/
	m = (y1 - y2) / (GLfloat)(x1 - x2);
	step_x = (x1 < x2) ? 1 : -1;
	step_y = (y1 < y2) ? 1 : -1;

	//step 2. 根据斜率绝对值分别沿x或者y轴按步长变化
	if (fabs(m) < 1.0) {
		p_k = 2 * delta_y - delta_x;
		glBegin(GL_POINTS);
		glVertex2i(x_k, y_k);

		writePixel(x_k, y_k);
		for (int i = 0; i < delta_x; i++) {
			if (p_k > 0) {
				y_k += step_y;	x_k += step_x;
				p_k = p_k + 2 * delta_y - 2 * delta_x;
			}
			else {
				x_k += step_x;
				p_k = p_k + 2 * delta_y;
			}
			glVertex2i(x_k, y_k);

			writePixel(x_k, y_k);
		}
		glEnd();
	}
	else {
		p_k = 2 * delta_x - delta_y;
		glBegin(GL_POINTS);
		glVertex2i(x_k, y_k);

		writePixel(x_k, y_k);
		for (int i = 0; i < delta_y; i++) {
			if (p_k > 0) {
				y_k += step_y;	x_k += step_x;
				p_k = p_k + 2 * delta_x - 2 * delta_y;
			}
			else {
				y_k += step_y;
				p_k = p_k + 2 * delta_x;
			}
			glVertex2i(x_k, y_k);

			writePixel(x_k, y_k);
		}
		glEnd();
	}
}



void drawCircleBresenham(int ctrx, int ctry, float r)
{
	GLint x_k, y_k;
	GLfloat p_k;

	x_k = 0;
	y_k = r;
	p_k = 5.0f / 4.0f - r;

	glBegin(GL_POINTS);
	while (x_k <= y_k) {
		glVertex2i(ctrx + x_k, ctry + y_k);
		glVertex2i(ctrx + y_k, ctry + x_k);
		glVertex2i(ctrx - x_k, ctry + y_k);
		glVertex2i(ctrx - y_k, ctry + x_k);
		glVertex2i(ctrx + x_k, ctry - y_k);
		glVertex2i(ctrx + y_k, ctry - x_k);
		glVertex2i(ctrx - x_k, ctry - y_k);
		glVertex2i(ctrx - y_k, ctry - x_k);

		writePixel(ctrx + x_k, ctry + y_k);
		writePixel(ctrx + y_k, ctry + x_k);
		writePixel(ctrx - x_k, ctry + y_k);
		writePixel(ctrx - y_k, ctry + x_k);
		writePixel(ctrx + x_k, ctry - y_k);
		writePixel(ctrx + y_k, ctry - x_k);
		writePixel(ctrx - x_k, ctry - y_k);
		writePixel(ctrx - y_k, ctry - x_k);

		if (p_k < 0) {
			p_k = p_k + 2 * x_k + 3;
			x_k++;
		}
		else {
			x_k++;
			p_k = p_k + 2 * x_k + 3 - 2 * y_k + 2;
			y_k--;
		}
	}
	glEnd();
}



void drawEllipseBresenham(int ctrx, int ctry, int a, int b)
{
	GLfloat p_k;
	GLint a_2, b_2;		//a^2, b^2
	GLint x_k, y_k;

	a_2 = a * a;	b_2 = b * b;
	p_k = b_2 - a_2*b + a_2 / 4.0;		//p_0
	x_k = 0;	y_k = b;	//(x0, y0) = (0, b)


	//area 1, |k| < 1
	glBegin(GL_POINTS);
	while (b_2 * x_k <= a_2 * y_k) {
		glVertex2i(ctrx + x_k, ctry + y_k);
		glVertex2i(ctrx - x_k, ctry + y_k);
		glVertex2i(ctrx + x_k, ctry - y_k);
		glVertex2i(ctrx - x_k, ctry - y_k);

		writePixel(ctrx + x_k, ctry + y_k);
		writePixel(ctrx - x_k, ctry + y_k);
		writePixel(ctrx + x_k, ctry - y_k);
		writePixel(ctrx - x_k, ctry - y_k);
		if (p_k < 0) {
			p_k = p_k + 2 * b_2 * x_k + 3 * b_2;
			x_k++;
		}
		else {
			p_k = p_k + 2 * b_2 * x_k + 3 * b_2 - 2 * a_2 * y_k + 2 * a_2;
			x_k++; y_k--;
		}
	}
	glEnd();

	//area 2, |k| >= 1
	p_k = b_2 * (x_k + 1.0 / 2.0) * (x_k + 1.0 / 2.0) + a_2 * (y_k - 1) * (y_k - 1) - a_2 * b_2;
	glBegin(GL_POINTS);
	while (y_k >= 0) {
		glVertex2i(ctrx + x_k, ctry + y_k);
		glVertex2i(ctrx - x_k, ctry + y_k);
		glVertex2i(ctrx + x_k, ctry - y_k);
		glVertex2i(ctrx - x_k, ctry - y_k);

		writePixel(ctrx + x_k, ctry + y_k);
		writePixel(ctrx - x_k, ctry + y_k);
		writePixel(ctrx + x_k, ctry - y_k);
		writePixel(ctrx - x_k, ctry - y_k);
		if (p_k > 0) {
			p_k = p_k - 2 * a_2 * y_k + 3 * a_2;
			y_k--;
		}
		else {
			p_k = p_k + 2 * b_2 * x_k + 3 * a_2 - 2 * a_2 * y_k + 2 * b_2;
			y_k--;   x_k++;
		}
	}
	glEnd();

}

void drawRotateEllipse(int ctrx, int ctry, int a, int b, double theta)
{
	//double x

	//Bezir_2();
}

void drawPolygon(std::vector<CPoint>& ptVec)
{
	CPoint pt1, pt2;
	int size = ptVec.size();
	for (int i = 0; i < size; i++) {
		pt1 = ptVec[i];
		pt2 = ptVec[(i + 1) % size];
		drawLineBresenham(pt1.x, pt1.y, pt2.x, pt2.y);
	}
}


typedef struct Edge
{
	float x;
	float dx;
	int ymax;
	Edge *next;
};

void polygonScan(std::vector<CPoint> &ptVec)
{
	if (ptVec.size() == 0)
		return;
	int maxY = ptVec.front().y;
	int minY = maxY;
	for_each(ptVec.begin(), ptVec.end(),
		[&](CPoint &p) {maxY = max(maxY, p.y); minY = min(minY, p.y); }
	);

	int height = maxY - minY + 1;
	//初始化ET和AET
	Edge **pET = new Edge*[height];
	for (int i = 0; i < height; i++)
	{
		pET[i] = new Edge();
		pET[i]->next = NULL;
	}
	Edge *AET;
	AET = new Edge();
	AET->next = NULL;


	glBegin(GL_POINTS);

	//建立边表NET
	for (int i = 0; i<ptVec.size(); i++) {
		CPoint p0 = ptVec[(i - 1 + ptVec.size()) % ptVec.size()];
		CPoint p1 = ptVec[i];
		CPoint p2 = ptVec[(i + 1 + ptVec.size()) % ptVec.size()];
		CPoint p3 = ptVec[(i + 2 + ptVec.size()) % ptVec.size()];

		//舍弃水平线
		if (p1.y == p2.y)
			continue;

		int ymin = min(p1.y, p2.y);
		int ymax = max(p1.y, p2.y);
		float x = p1.y > p2.y ? p2.x : p1.x;
		float dx = (p1.x - p2.x) * 1.0f / (p1.y - p2.y);

		//处理非极值点（左右极值点）
		if (((p1.y < p2.y) && (p1.y > p0.y)) || ((p2.y < p1.y) && (p2.y > p3.y)))
		{
			ymin++;
			x += dx;
		}

		Edge *p = new Edge();
		p->ymax = ymax;
		p->x = x;
		p->dx = dx;
		p->next = pET[ymin - minY]->next;
		pET[ymin - minY]->next = p;
	}

	//从下往上扫描，y步长为1
	for (int i = 0; i < height; i++)
	{
		//取出ET中当前扫描行的所有边并按x的递增顺序（若x相等则按dx的递增顺序）插入AET
		while (pET[i]->next)
		{
			//取出ET中当前扫描行表头位置的边
			Edge *pInsert = pET[i]->next;
			Edge *p = AET;
			//在AET中搜索合适的插入位置
			while (p->next)
			{
				if (pInsert->x > p->next->x)
				{
					p = p->next;
					continue;
				}
				if (pInsert->x == p->next->x && pInsert->dx > p->next->dx)
				{
					p = p->next;
					continue;
				}
				//找到位置
				break;
			}
			//将pInsert从ET中删除，并插入AET的当前位置
			pET[i]->next = pInsert->next;
			pInsert->next = p->next;
			p->next = pInsert;
		}

		//AET中的边两两配对并填色
		Edge *p = AET;
		while (p->next && p->next->next) {
			for (int x = p->next->x; x < p->next->next->x; x++) {
				glVertex2i(x, i + minY);
			}
			p = p->next->next;
		}

		//删除AET中满足y=ymax的边
		p = AET;
		while (p->next) {
			if (p->next->ymax == (i + minY)) {
				Edge *pDelete = p->next;
				p->next = pDelete->next;
				pDelete->next = nullptr;
				delete pDelete;
			}
			else {
				p = p->next;
			}
		}

		//更新AET中边的x值，进入下一循环
		p = AET;
		while (p->next) {
			p->next->x += p->next->dx;
			p = p->next;
		}
	}
	glEnd();
}


int direction_8[] = { 1, 0, -1, 0, 0, 1, 0, -1,  1, 1, 1, -1, -1, 1, -1, -1 };

void boundaryFill4(int x, int y, COLORREF boundaryColor, COLORREF fillColor)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT)	return;
	COLORREF curColor;
	curColor = readPixel(x - 500, y - 500);

	if (curColor != boundaryColor && curColor != fillColor) {
		glVertex2i(x - WIDTH / 2, y - HEIGHT / 2);
		writePixel(x - 500, y - 500);
		for (int i = 0; i < sizeof(direction_8) / sizeof(int) / 2; i += 2)
			boundaryFill4(x + direction_8[i], y + direction_8[i + 1], fillColor, boundaryColor);
	}
}

void boundaryFill8(int x, int y, COLORREF boundaryColor, COLORREF fillColor)
{
	COLORREF curColor;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_UNSIGNED_BYTE, &curColor);
	if (curColor != boundaryColor && curColor != fillColor) {
		glVertex2i(x, y);
		for (int i = 0; i < sizeof(direction_8) / sizeof(int); i += 2)
			boundaryFill4(x + direction_8[i], y + direction_8[i + 1], fillColor, boundaryColor);
	}
}


void scanLineSeedFill(int x, int y, COLORREF boundaryColor, COLORREF fillColor)
{
	std::stack<CPoint> ptStack;

	glPointSize(1.0);
	glColor3ub(GetRValue(fillColor), GetGValue(fillColor), GetBValue(fillColor));
	setColor(fillColor);
	glBegin(GL_POINTS);

	ptStack.push(CPoint(x, y));
	while (!ptStack.empty()) {
		int left, right;
		CPoint pt = ptStack.top();	ptStack.pop();

		//往左填充
		for (left = pt.x; readPixel(left, pt.y) != boundaryColor && readPixel(left, pt.y) != fillColor; left--) {
			glVertex2i(left, pt.y);
			writePixel(left, pt.y);
		}
		left++;
		//往右填充
		for (right = pt.x + 1; readPixel(right, pt.y) != boundaryColor && readPixel(right, pt.y) != fillColor; right++) {
			glVertex2i(right, pt.y);
			writePixel(right, pt.y);
		}
		right--;

		//寻找y-1 和 y+1的种子点
		int i;
		if (pt.y > -HEIGHT / 2 && pt.y < HEIGHT / 2) {
			for (i = left + 1; i < right; i++) {
				if (readPixel(i, pt.y - 1) != boundaryColor && readPixel(i, pt.y - 1) != fillColor) {
					ptStack.push(CPoint(i, pt.y - 1));
					break;
				}
			}
			for (i = left + 1; i < right; i++) {
				if (readPixel(i, pt.y + 1) != boundaryColor && readPixel(i, pt.y + 1) != fillColor) {
					ptStack.push(CPoint(i, pt.y + 1));
					break;
				}
			}
		}

	}
	glEnd();
}




//通过向量叉积，判断点是否在多边形里面
int PtInRegion(CPoint &sClipPt, CPoint &eClipPt, CPoint &point)
{
	int result = (eClipPt.x - sClipPt.x) * (point.y - sClipPt.y) - (eClipPt.y - sClipPt.y) * (point.x - sClipPt.x);
	return (result > 0) ? 0 : 1;
}

//直线求交点
bool intersect(CPoint &p0, CPoint &p1, CPoint &q0, CPoint &q1, CPoint &cross)
{
	//float d = p0 * q1 - q0 * p1;
	//未考虑特殊情形

	int a0 = p0.y - p1.y;
	int b0 = p1.x - p0.x;
	int c0 = p0.x * p1.y - p0.y * p1.x;

	int a1 = q0.y - q1.y;
	int b1 = q1.x - q0.x;
	int c1 = q0.x * q1.y - q0.y * q1.x;

	int d = a0 * b1 - a1 * b0;
	if (d == 0)	return false;

	cross.x = (b0 * c1 - b1 * c0) * 1.0 / d;
	cross.y = (a1 * c0 - a0 * c1) * 1.0 / d;


	return true;
}

int Sutherland_Hodgman_clip(std::vector<CPoint> &srcPoly, std::vector<CPoint> &clipPoly)
{
	int i, j, k;
	bool curPtIn, prePtIn;
	CPoint sPt, ePt, sClipPt, eClipPt;
	std::vector<CPoint> destPoly;

	sClipPt = clipPoly.back();

	for (i = 0; i < clipPoly.size(); i++) {
		eClipPt = clipPoly[i];

		sPt = ePt = srcPoly.back();
		curPtIn = PtInRegion(sClipPt, eClipPt, sPt);

		for (j = 0; j < srcPoly.size(); j++) {
			sPt = ePt;
			ePt = srcPoly[j];
			prePtIn = curPtIn;
			curPtIn = PtInRegion(sClipPt, eClipPt, ePt);
			if (curPtIn == true) {
				//从外到内：输出顶点表依次加入交点和第二点
				if (prePtIn == false) {
					CPoint cross;
					if (intersect(sPt, ePt, sClipPt, eClipPt, cross))
						destPoly.push_back(cross);
				}
				destPoly.push_back(ePt);
			}
			else if (prePtIn == true) {
				//从内到外：只加入交点
				CPoint cross;
				if (intersect(sPt, ePt, sClipPt, eClipPt, cross))
					destPoly.push_back(cross);
			}
		}
		sClipPt = eClipPt;
		srcPoly.clear();
		srcPoly.assign(destPoly.begin(), destPoly.end());
		destPoly.clear();
	}

	return 0;
}

void Bezir_2(CPoint &start, CPoint &control, CPoint &end)
{
	//首先根据三个点的位置，确定要画的点数
	int len1 = sqrt((start.x - control.x) * (start.x - control.x) + (start.y - control.y) * (start.y - control.y));
	int len2 = sqrt((end.x - control.x) * (end.x - control.x) + (end.y - control.y) * (end.y - control.y));
	int n = (len1 + len2)*2;

	glBegin(GL_POINTS);
	float coef[3];
	for (int i = 0; i <= n; i++) {
		float u = i * 1.0 / n;
		coef[0] = (1.0 - u) * (1.0 - u);
		coef[1] = 2 * (1.0 - u) * u;
		coef[2] = u * u;
		float x, y;
		x = coef[0] * start.x + coef[1] * control.x + coef[2] * end.x;
		y = coef[0] * start.y + coef[1] * control.y + coef[2] * end.y;
		glVertex2i(round(x), round(y));
	}
	glEnd();
}

void Bezir_3(CPoint & start, CPoint & end, CPoint & ctrl1, CPoint & ctrl2)
{
	//根据四点计算样本点数，根据实际情况，点会分布不均，所以乘以2
	int len1 = sqrt((start.x - ctrl1.x) * (start.x - ctrl1.x) + (start.y - ctrl1.y) * (start.y - ctrl1.y));
	int len2 = sqrt((ctrl2.x - ctrl1.x) * (ctrl2.x - ctrl1.x) + (ctrl2.y - ctrl1.y) * (ctrl2.y - ctrl1.y));
	int len3 = sqrt((end.x - ctrl2.x) * (end.x - ctrl2.x) + (end.y - ctrl2.y) * (end.y - ctrl2.y));
	int n = (len1 + len2 + len3) * 2;

	glBegin(GL_POINTS);
	float coef[4];
	for (int i = 0; i <= n; i++) {
		float u = i * 1.0 / n;
		coef[0] = (1.0 - u) * (1.0 - u) * (1.0 - u);
		coef[1] = 3 * (1.0 - u) * (1.0 - u) * u;
		coef[2] = 3 * (1.0 - u) * u * u;
		coef[3] = u * u * u;
		float x, y;
		x = coef[0] * start.x + coef[1] * ctrl1.x + coef[2] * ctrl2.x+ coef[3] * end.x;
		y = coef[0] * start.y + coef[1] * ctrl1.y + coef[2] * ctrl2.y + coef[3] * end.y;
		glVertex2i(round(x), round(y));
	}
	glEnd();
}

void Bezir_n(std::vector<CPoint> &ptVec)
{
	int size = ptVec.size();
	if (size < 2)	return;
	//n个顶点，有2n-2个控制点，除去第一个顶点和和最后一个顶点之间的两个控制点
	int nCtrlPt = size * 2 - 2;
	CPoint *pctrlPt = new CPoint[nCtrlPt];

	//初始化第一个和最后一个控制点
	pctrlPt[0] = ptVec.front();
	pctrlPt[nCtrlPt - 1] = ptVec.back();

	//E、F分别为AB，BC中点，FD/DE = BC/AB;
	CPoint D, E, F;
	float AB, BC, k;
	CPoint vecDB;
	CPoint A = ptVec[0];
	CPoint B = ptVec[1];
	CPoint C = ptVec[2%size];
	AB = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
	E.SetPoint((A.x + B.x) / 2, (A.y + B.y) / 2);
	//循环第2个顶点至第n-1个顶点，计算出每个顶点的2个控制点
	for (int i = 1; i < size-1; i++) {
		C = ptVec[i+1];
		F.SetPoint((C.x + B.x) / 2, (C.y + B.y) / 2);
		BC = sqrt((C.x - B.x) * (C.x - B.x) + (C.y - B.y) * (C.y - B.y));
		k = AB / (BC+AB);
		D.SetPoint(E.x + (F.x - E.x) * k, E.y + (F.y - E.y) * k);
		vecDB = B - D;
		pctrlPt[2 * i - 1].SetPoint(E.x + vecDB.x, E.y + vecDB.y);
		pctrlPt[2 * i].SetPoint(F.x + vecDB.x, F.y + vecDB.y);

		//准备下一次循环
		AB = BC;
		E = F;
		A = B; B = C;
	}

	//使用三阶Bezir函数画线：两个顶点和中间的两个控制点
	for (int i = 0; i < size - 1; i++) {
		Bezir_3(ptVec[i], ptVec[i+1], pctrlPt[2*i], pctrlPt[2*i+1]);
	}

	delete pctrlPt;
}