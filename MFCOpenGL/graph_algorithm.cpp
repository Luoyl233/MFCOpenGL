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

	//ѡȡ��һ����Ϊ���
	x_k = x1;
	y_k = y1;


	/*
	*step 1. �ж���x��y�����Ե�λ�����仯
	*x1 > x2��x����϶���������
	*x2 < x2, x����϶��Ǽ��ٵ�
	*/
	m = (y1 - y2) / (GLfloat)(x1 - x2);
	step_x = (x1 < x2) ? 1 : -1;
	step_y = (y1 < y2) ? 1 : -1;

	//step 2. ����б�ʾ���ֵ�ֱ���x����y�ᰴ�����仯
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
	//��ʼ��ET��AET
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

	//�����߱�NET
	for (int i = 0; i<ptVec.size(); i++) {
		CPoint p0 = ptVec[(i - 1 + ptVec.size()) % ptVec.size()];
		CPoint p1 = ptVec[i];
		CPoint p2 = ptVec[(i + 1 + ptVec.size()) % ptVec.size()];
		CPoint p3 = ptVec[(i + 2 + ptVec.size()) % ptVec.size()];

		//����ˮƽ��
		if (p1.y == p2.y)
			continue;

		int ymin = min(p1.y, p2.y);
		int ymax = max(p1.y, p2.y);
		float x = p1.y > p2.y ? p2.x : p1.x;
		float dx = (p1.x - p2.x) * 1.0f / (p1.y - p2.y);

		//����Ǽ�ֵ�㣨���Ҽ�ֵ�㣩
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

	//��������ɨ�裬y����Ϊ1
	for (int i = 0; i < height; i++)
	{
		//ȡ��ET�е�ǰɨ���е����б߲���x�ĵ���˳����x�����dx�ĵ���˳�򣩲���AET
		while (pET[i]->next)
		{
			//ȡ��ET�е�ǰɨ���б�ͷλ�õı�
			Edge *pInsert = pET[i]->next;
			Edge *p = AET;
			//��AET���������ʵĲ���λ��
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
				//�ҵ�λ��
				break;
			}
			//��pInsert��ET��ɾ����������AET�ĵ�ǰλ��
			pET[i]->next = pInsert->next;
			pInsert->next = p->next;
			p->next = pInsert;
		}

		//AET�еı�������Բ���ɫ
		Edge *p = AET;
		while (p->next && p->next->next) {
			for (int x = p->next->x; x < p->next->next->x; x++) {
				glVertex2i(x, i + minY);
			}
			p = p->next->next;
		}

		//ɾ��AET������y=ymax�ı�
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

		//����AET�бߵ�xֵ��������һѭ��
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

		//�������
		for (left = pt.x; readPixel(left, pt.y) != boundaryColor && readPixel(left, pt.y) != fillColor; left--) {
			glVertex2i(left, pt.y);
			writePixel(left, pt.y);
		}
		left++;
		//�������
		for (right = pt.x + 1; readPixel(right, pt.y) != boundaryColor && readPixel(right, pt.y) != fillColor; right++) {
			glVertex2i(right, pt.y);
			writePixel(right, pt.y);
		}
		right--;

		//Ѱ��y-1 �� y+1�����ӵ�
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




//ͨ������������жϵ��Ƿ��ڶ��������
int PtInRegion(CPoint &sClipPt, CPoint &eClipPt, CPoint &point)
{
	int result = (eClipPt.x - sClipPt.x) * (point.y - sClipPt.y) - (eClipPt.y - sClipPt.y) * (point.x - sClipPt.x);
	return (result > 0) ? 0 : 1;
}

//ֱ���󽻵�
bool intersect(CPoint &p0, CPoint &p1, CPoint &q0, CPoint &q1, CPoint &cross)
{
	//float d = p0 * q1 - q0 * p1;
	//δ������������

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
				//���⵽�ڣ������������μ��뽻��͵ڶ���
				if (prePtIn == false) {
					CPoint cross;
					if (intersect(sPt, ePt, sClipPt, eClipPt, cross))
						destPoly.push_back(cross);
				}
				destPoly.push_back(ePt);
			}
			else if (prePtIn == true) {
				//���ڵ��⣺ֻ���뽻��
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
	//���ȸ����������λ�ã�ȷ��Ҫ���ĵ���
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
	//�����ĵ������������������ʵ����������ֲ����������Գ���2
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
	//n�����㣬��2n-2�����Ƶ㣬��ȥ��һ������ͺ����һ������֮����������Ƶ�
	int nCtrlPt = size * 2 - 2;
	CPoint *pctrlPt = new CPoint[nCtrlPt];

	//��ʼ����һ�������һ�����Ƶ�
	pctrlPt[0] = ptVec.front();
	pctrlPt[nCtrlPt - 1] = ptVec.back();

	//E��F�ֱ�ΪAB��BC�е㣬FD/DE = BC/AB;
	CPoint D, E, F;
	float AB, BC, k;
	CPoint vecDB;
	CPoint A = ptVec[0];
	CPoint B = ptVec[1];
	CPoint C = ptVec[2%size];
	AB = sqrt((A.x - B.x) * (A.x - B.x) + (A.y - B.y) * (A.y - B.y));
	E.SetPoint((A.x + B.x) / 2, (A.y + B.y) / 2);
	//ѭ����2����������n-1�����㣬�����ÿ�������2�����Ƶ�
	for (int i = 1; i < size-1; i++) {
		C = ptVec[i+1];
		F.SetPoint((C.x + B.x) / 2, (C.y + B.y) / 2);
		BC = sqrt((C.x - B.x) * (C.x - B.x) + (C.y - B.y) * (C.y - B.y));
		k = AB / (BC+AB);
		D.SetPoint(E.x + (F.x - E.x) * k, E.y + (F.y - E.y) * k);
		vecDB = B - D;
		pctrlPt[2 * i - 1].SetPoint(E.x + vecDB.x, E.y + vecDB.y);
		pctrlPt[2 * i].SetPoint(F.x + vecDB.x, F.y + vecDB.y);

		//׼����һ��ѭ��
		AB = BC;
		E = F;
		A = B; B = C;
	}

	//ʹ������Bezir�������ߣ�����������м���������Ƶ�
	for (int i = 0; i < size - 1; i++) {
		Bezir_3(ptVec[i], ptVec[i+1], pctrlPt[2*i], pctrlPt[2*i+1]);
	}

	delete pctrlPt;
}