#include "stdafx.h"
#include "Point3D.h"


Point3D::Point3D()
{
	x = y = z = 0.0;
}

Point3D::Point3D(const double & x, const double & y, const double & z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


Point3D::~Point3D()
{
}

Point3D::Point3D(const Point3D & pt)
{
	x = pt.x;
	y = pt.y;
	z = pt.z;
}

Point3D & Point3D::operator= (const Point3D & pt)
{
	// TODO: 在此处插入 return 语句
	x = pt.x;
	y = pt.y;
	z = pt.z;

	return (*this);
}

Point3D Point3D::operator+ (const Point3D & pt)
{
	Point3D result = pt;
	result.x += x;
	result.y += y;
	result.z += z;

	return result;
}

Point3D Point3D::operator-(const Point3D & pt)
{
	Point3D result = pt;
	result.x -= x;
	result.y -= y;
	result.z -= z;

	return result;
}
