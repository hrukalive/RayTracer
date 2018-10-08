/*
  ==============================================================================

    Utility.cpp
    Created: 6 Oct 2018 4:29:50pm
    Author:  bowen

  ==============================================================================
*/

#include "Utility.h"

std::ostream& operator<<(std::ostream& o, const Vec3D& vec)
{
    o << vec.x << ", " << vec.y << ", " << vec.z;
    return o;
}

std::ostream& operator<<(std::ostream& o, const Matrix& m)
{
	o << m(0, 0) << " " << m(0, 1) << " " << m(0, 2) << " " << m(0, 3) << std::endl;
	o << m(1, 0) << " " << m(1, 1) << " " << m(1, 2) << " " << m(1, 3) << std::endl;
	o << m(2, 0) << " " << m(2, 1) << " " << m(2, 2) << " " << m(2, 3) << std::endl;
	o << m(3, 0) << " " << m(3, 1) << " " << m(3, 2) << " " << m(3, 3);
	return o;
}

bool operator<(const Point3D& lhs, const Point3D& rhs)
{
	return lhs.x < rhs.x && lhs.y < rhs.y && lhs.z < rhs.z;
}
