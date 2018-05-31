#pragma once



class Point3D
{
public:
	Point3D();
	Point3D(const double &x, const double &y, const double &z);
	~Point3D();
	Point3D(const Point3D& pt);
	Point3D& operator= (const Point3D& pt);
	Point3D operator+ (const Point3D& pt);
	Point3D operator- (const Point3D& pt);

public:
	double x, y, z;
};

