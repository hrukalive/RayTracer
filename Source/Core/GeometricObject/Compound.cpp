#include "Compound.h"
#include "BBox.h"

void Compound::UpdateBoundingBox()
{
    if (objects.size() == 1)
        boundingBox = objects[0]->GetBoundingBox();
    else if (objects.size() > 1)
        boundingBox.Merge(objects[objects.size() - 1]->GetBoundingBox());
}

void Compound::AddObject(std::shared_ptr<GeometricObject> obj)
{
    objects.push_back(obj);
    UpdateBoundingBox();
}

void Compound::SetMaterial(std::shared_ptr<Material> newMaterialPtr)
{
    for (auto& obj : objects)
        obj->SetMaterial(newMaterialPtr);
}

HitRecord Compound::Hit(const Ray& ray)
{
    HitRecord record;
    FP_TYPE tmin = INFINITY;

    if (!boundingBox.Hit(ray).Hit)
        return record;

    for (int i = 0; i < objects.size(); i++)
    {
        HitRecord tmp = objects[i]->Hit(ray);
        if (tmp.Hit && tmp.T < tmin)
        {
            tmin = tmp.T;
            record = tmp;
        }
    }
    return record;
}