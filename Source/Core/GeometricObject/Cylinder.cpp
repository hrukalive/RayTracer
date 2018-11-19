/*
  ==============================================================================

    Cylinder.cpp
    Created: 17 Nov 2018 9:42:58pm
    Author:  bowen

  ==============================================================================
*/

#include "Cylinder.h"
#include "OpenCylinder.h"
#include "Disk.h"

void Cylinder::UpdateBoundingBox()
{
    Point3D deltar(r, r, r);
    Point3D deltay(0, h, 0);
    boundingBox = BBox(bc - deltar, bc + deltar + deltay);
}

Cylinder::Cylinder(Point3D bottomCenter, FP_TYPE radius, FP_TYPE height) : bc(bottomCenter), r(radius), h(height)
{
    auto disk1 = std::shared_ptr<Disk>{ new Disk(bottomCenter, radius, -Y_DIR) };
    Point3D deltay(0, h, 0);
    auto disk2 = std::shared_ptr<Disk>{ new Disk(bottomCenter + deltay, radius, Y_DIR) };
    auto opencylinder = std::shared_ptr<OpenCylinder>{ new OpenCylinder(bottomCenter, radius, height) };
    AddObject(disk1);
    AddObject(disk2);
    AddObject(opencylinder);
}