/*
  ==============================================================================

    OBJParser.h
    Created: 4 Oct 2018 4:05:15pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "GeometricObject/GeometricObject.h"
#include "GeometricObject/MeshTriangle.h"
#include "GeometricObject/Mesh.h"

class OBJParser
{
public:
    Mesh parse(StringArray& content)
    {
        std::vector<Point3D> vertices;
        std::vector<MeshTriangle> faces;
        std::vector<Vec3D> normals;
        
        for (auto& line : content)
        {
            if (line[0] == 'v')
            {
                std::string str = line.toStdString();
                std::stringstream ss(str);
                char c;
                FP_TYPE v1, v2, v3;
                ss >> c >> v1 >> v2 >> v3;
                vertices.push_back(Point3D(v1, v2, v3));
            }
            else if (line[0] == 'f')
            {
                std::string str = line.toStdString();
                std::stringstream ss(str);
                char c;
                int v1, v2, v3;
                ss >> c >> v1 >> v2 >> v3;
            }
        }
        return Mesh();
    }
};
