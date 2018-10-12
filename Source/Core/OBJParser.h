/*
  ==============================================================================

    OBJParser.h
    Created: 4 Oct 2018 4:05:15pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include <sstream>
#include "GeometricObject/GeometricObject.h"
#include "GeometricObject/Mesh.h"

class OBJParser
{
public:
    Mesh parse(StringArray& content)
    {
        std::vector<Point3D> vertices;
        std::vector<Vector3D<int>> faces;
        std::vector<Vec3D> faceNormals;
		std::vector<Vec3D> vertexNormals;
        
        for (auto& line : content)
        {
            if (line[0] == 'v')
            {
                std::string str = line.toStdString();
				std::stringstream ss(str);
				std::string c;
                FP_TYPE v1, v2, v3;
                ss >> c >> v1 >> v2 >> v3;
                vertices.push_back(Point3D(v1, v2, v3));
            }
            else if (line[0] == 'f')
            {
                std::string str = line.toStdString();
                std::stringstream ss(str);
				std::string c;
                int v1, v2, v3;
                ss >> c >> v1 >> v2 >> v3;
				v1--; v2--; v3--;
				faces.push_back(Vector3D<int>(v1, v2, v3));
				faceNormals.push_back((vertices[v2] - vertices[v1]) ^ (vertices[v3] - vertices[v1]));
            }
        }
		vertexNormals.resize(vertices.size());
		for (int i = 0; i < faces.size(); i++)
		{
			auto& face = faces[i];
			vertexNormals[face.x] += faceNormals[i];
			vertexNormals[face.y] += faceNormals[i];
			vertexNormals[face.z] += faceNormals[i];
		}
		for (auto& vertexNormal : vertexNormals)
		{
			vertexNormal = vertexNormal.normalised();
		}

		std::vector<MeshTriangle> triangles;
		Mesh mesh;
		for (int i = 0; i < faces.size(); i++)
		{
			auto& face = faces[i];
			mesh.AddObject(std::dynamic_pointer_cast<GeometricObject>(
				std::make_shared<MeshTriangle>(MeshTriangle(vertices[face.x], vertices[face.y], vertices[face.z], 
					vertexNormals[face.x], vertexNormals[face.y], vertexNormals[face.z]))));
		}
		mesh.Setup();
        return mesh;
    }
};
