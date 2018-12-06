/*
  ==============================================================================

    GeometricObject.cpp
    Created: 18 Nov 2018 4:20:09pm
    Author:  bowen

  ==============================================================================
*/

#include "GeometricObject.h"
#include "Basic/Cylinder.h"
#include "Compound.h"
#include "Grid.h"
#include "Instance.h"
#include "Mesh.h"
#include "../Utility/OBJParser.h"
#include "../Globals.h"

void GeometricObject::SetMaterial(std::shared_ptr<Material> newMaterialPtr) {
    materialPtr = newMaterialPtr;
}

BBox& GeometricObject::GetBoundingBox() {
    return boundingBox;
}

std::vector<std::pair<Point3D, Vec3D>> GeometricObject::Sample(int numSamples)
{
    return std::vector<std::pair<Point3D, Vec3D>>();
}

std::pair<Point3D, Vec3D> GeometricObject::SampleSingle()
{
    return std::make_pair(ZERO, ZERO);
}

FP_TYPE GeometricObject::pdf(const HitRecord& record)
{
    auto diff = boundingBox.GetMaxPoint() - boundingBox.GetMinPoint();
    return 1.0 / (2 * (diff.x * diff.y + diff.y * diff.z + diff.x * diff.z));
}

std::vector<HitRecord> GeometricObject::getHitInterval(const Ray& ray)
{
    BBox box = world->GetBoundingBox();
    FP_TYPE dist = std::max((ray.Origin - box.GetMinPoint()).length(), (ray.Origin - box.GetMaxPoint()).length());
    dist += (box.GetMaxPoint() - box.GetMinPoint()).length();

    std::vector<HitRecord> ret;
    FP_TYPE tmpT = -dist;
    Ray tmpRay = ray;
    FP_TYPE acc = -dist;
    while (true)
    {
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        HitRecord record = Hit(tmpRay);
        if (!record.Hit)
            return ret;
        tmpT = record.T;
        record.T += acc;
        acc = record.T;
        ret.push_back(record);
        tmpRay = Ray(tmpRay.GetPoint(tmpT + kEpsilon), ray.Direction);
        record = Hit(tmpRay);
        if (!record.Hit)
        {
            ret.push_back(ret[ret.size() - 1]);
            return ret;
        }
        else
        {
            tmpT = record.T;
            record.T += acc;
            acc = record.T;
            ret.push_back(record);
        }
    }
    return ret;
}

std::shared_ptr<GeometricObject> GeometricObject::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (cmd[2] == "Box")
        return Box::parse(cmd, env);
    else if (cmd[2] == "CSG")
        return CSG::parse(cmd, env);
    else if (cmd[2] == "Cylinder")
        return Cylinder::parse(cmd, env);
    else if (cmd[2] == "Disk")
        return Disk::parse(cmd, env);
    else if (cmd[2] == "OpenCylinder")
        return OpenCylinder::parse(cmd, env);
    else if (cmd[2] == "Rectangle")
        return RayTracer::Rectangle::parse(cmd, env);
    else if (cmd[2] == "Sphere")
        return Sphere::parse(cmd, env);
    else if (cmd[2] == "Torus")
        return Torus::parse(cmd, env);
    else if (cmd[2] == "Triangle")
        return Triangle::parse(cmd, env);
    else if (cmd[2] == "Compound")
        return Compound::parse(cmd, env);
    else if (cmd[2] == "Grid")
        return RayTracer::Grid::parse(cmd, env);
    else if (cmd[2] == "Instance")
        return Instance::parse(cmd, env);
    else if (cmd[2] == "Mesh")
        return Mesh::parse(cmd, env);
    else
        throw std::invalid_argument("[GEOMETRY] Type not found.");
}

std::shared_ptr<GeometricObject> Box::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Box>(Box(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())),
        Point3D(
            std::stod(cmd[7].toStdString()),
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString()))));
}

std::shared_ptr<GeometricObject> CSG::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<CSG>(CSG());
    for (int i = 4; i < cmd.size(); i += 2)
    {
        if (cmd[i] == "OBJ")
        {
            if (env.find(cmd[i + 1]) == env.end())
                throw std::invalid_argument("[CSG] Required geometry pointer not found.");
            ret->addObject(std::static_pointer_cast<GeometricObject>(env[cmd[i + 1]]));
        }
        else if (cmd[i] == "OP")
        {
            if (cmd[i + 1] == "INTERSECTION")
                ret->addOperation(OpType::INTERSECTION);
            else if (cmd[i + 1] == "UNION")
                ret->addOperation(OpType::UNION);
            else if (cmd[i + 1] == "DIFFERENCE")
                ret->addOperation(OpType::DIFFERENCE);
            else
                throw std::invalid_argument("[CSG] Invalid operation.");
        }
    }
    ret->build();
    return ret;
}

std::shared_ptr<GeometricObject> Cylinder::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Cylinder>(Cylinder(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        ),
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString())));
}

std::shared_ptr<GeometricObject> Disk::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Disk>(Disk(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())),
        std::stod(cmd[7].toStdString()),
        Vec3D(
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString()),
            std::stod(cmd[10].toStdString()))));
}

std::shared_ptr<GeometricObject> OpenCylinder::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<OpenCylinder>(OpenCylinder(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        ),
        std::stod(cmd[7].toStdString()),
        std::stod(cmd[8].toStdString())));
}

std::shared_ptr<GeometricObject> RayTracer::Rectangle::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<RayTracer::Rectangle>(RayTracer::Rectangle(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())),
        Vec3D(
            std::stod(cmd[7].toStdString()),
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString())),
        Vec3D(
            std::stod(cmd[10].toStdString()),
            std::stod(cmd[11].toStdString()),
            std::stod(cmd[12].toStdString()))));
}

std::shared_ptr<GeometricObject> Sphere::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Sphere>(Sphere(
        Point3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())
        ),
        std::stod(cmd[7].toStdString())));
}

std::shared_ptr<GeometricObject> Torus::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Torus>(Torus(
        std::stod(cmd[4].toStdString()),
        std::stod(cmd[5].toStdString())));
}

std::shared_ptr<GeometricObject> Triangle::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    return std::make_shared<Triangle>(Triangle(
        Vec3D(
            std::stod(cmd[4].toStdString()),
            std::stod(cmd[5].toStdString()),
            std::stod(cmd[6].toStdString())),
        Vec3D(
            std::stod(cmd[7].toStdString()),
            std::stod(cmd[8].toStdString()),
            std::stod(cmd[9].toStdString())),
        Vec3D(
            std::stod(cmd[10].toStdString()),
            std::stod(cmd[11].toStdString()),
            std::stod(cmd[12].toStdString()))));
}

std::shared_ptr<GeometricObject> Compound::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<Compound>(Compound());
    for (int i = 4; i < cmd.size(); i++)
    {
        if (env.find(cmd[i]) == env.end())
            throw std::invalid_argument("[COMPOUND] Required geometry pointer not found.");
        ret->AddObject(std::static_pointer_cast<GeometricObject>(env[cmd[i]]));
    }
    return ret;
}

std::shared_ptr<GeometricObject> RayTracer::Grid::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    auto ret = std::make_shared<RayTracer::Grid>(RayTracer::Grid());
    for (int i = 4; i < cmd.size(); i++)
    {
        if (env.find(cmd[i]) == env.end())
            throw std::invalid_argument("[GRID] Required geometry pointer not found.");
        ret->AddObject(std::static_pointer_cast<GeometricObject>(env[cmd[i]]));
    }
    ret->Setup();
    return ret;
}

std::shared_ptr<GeometricObject> Instance::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    if (env.find(cmd[4]) == env.end())
        throw std::invalid_argument("[INSTANCE] Required geometry pointer not found.");
    auto ret = std::make_shared<Instance>(Instance(std::static_pointer_cast<GeometricObject>(env[cmd[4]])));
    int i = 5;
    while (i < cmd.size())
    {
        if (cmd[i] == "ROTATEX")
        {
            ret->RotateX(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "ROTATEY")
        {
            ret->RotateY(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "ROTATEZ")
        {
            ret->RotateZ(std::stod(cmd[i + 1].toStdString()) * PI_OVER_180);
            i += 2;
        }
        else if (cmd[i] == "TRANSLATE")
        {
            ret->Translate(
                std::stod(cmd[i + 1].toStdString()), 
                std::stod(cmd[i + 2].toStdString()), 
                std::stod(cmd[i + 3].toStdString())
            );
            i += 4;
        }
        else if (cmd[i] == "SCALE")
        {
            ret->Scale(
                std::stod(cmd[i + 1].toStdString()),
                std::stod(cmd[i + 2].toStdString()),
                std::stod(cmd[i + 3].toStdString())
            );
            i += 4;
        }
        else
        {
            throw std::invalid_argument("[INSTANCE] Transformation not found.");
        }
    }
    return ret;
}

std::shared_ptr<GeometricObject> Mesh::parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env)
{
    File file(cmd[4]);
    if (!file.exists())
        throw std::invalid_argument("[MESH] Mesh file not found.");
    StringArray strarr;
    file.readLines(strarr);
    return std::make_shared<Mesh>(OBJParser().parse(strarr));
}
