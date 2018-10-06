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

bool operator<(const Point3D& lhs, const Point3D& rhs)
{
	return lhs.x < rhs.x || lhs.y < rhs.y || lhs.z < rhs.z;
}
