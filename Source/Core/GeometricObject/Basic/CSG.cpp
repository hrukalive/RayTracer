/*
  ==============================================================================

    CSG.cpp
    Created: 17 Nov 2018 12:21:47am
    Author:  bowen

  ==============================================================================
*/

#include "CSG.h"

std::vector<std::pair<HitRecord, bool>> CSG::mergeIntervals(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
{
    std::vector<std::pair<HitRecord, bool>> ret;
    for (auto& record : e1)
        ret.push_back(std::make_pair(record, false));
    for (auto& record : e2)
        ret.push_back(std::make_pair(record, true));
    std::sort(ret.begin(), ret.end(), [](const std::pair<HitRecord, bool>& lhs, const std::pair<HitRecord, bool>& rhs) -> bool {
        return lhs.first.T < rhs.first.T;
    });
    return ret;
}
std::vector<HitRecord> CSG::performUnion(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
{
    auto merged = mergeIntervals(e1, e2);
    std::vector<HitRecord> ret;
    bool inA = false, inB = false, prev = false, cur = false;
    for (auto& record : merged)
    {
        if (record.second == false) // op1
            inA = !inA;
        else // op2
            inB = !inB;
        cur = inA || inB;
        if (prev ^ cur)
            ret.push_back(record.first);
        prev = cur;
    }
    return ret;
}
std::vector<HitRecord> CSG::performIntection(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
{
    auto merged = mergeIntervals(e1, e2);
    std::vector<HitRecord> ret;
    bool inA = false, inB = false, prev = false, cur = false;
    for (auto& record : merged)
    {
        if (record.second == false) // op1
            inA = !inA;
        else // op2
            inB = !inB;
        cur = inA && inB;
        if (prev ^ cur)
            ret.push_back(record.first);
        prev = cur;
    }
    return ret;
}
std::vector<HitRecord> CSG::performDifference(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
{
    auto merged = mergeIntervals(e1, e2);
    std::vector<HitRecord> ret;
    bool inA = false, inB = true, prev = false, cur = false;
    for (auto& record : merged)
    {
        if (record.second == false) // op1
            inA = !inA;
        else // op2
            inB = !inB;
        cur = inA && inB;
        if (prev ^ cur)
            ret.push_back(record.first);
        prev = cur;
    }
    return ret;
}

void CSG::UpdateBoundingBox()
{
    bool first = true;
    for (auto& data : tree)
    {
        if (data.type == DataType::OBJECT)
        {
            if (first)
            {
                boundingBox = data.obj->GetBoundingBox();
                first = false;
            }
            else
            {
                boundingBox.Merge(data.obj->GetBoundingBox());
            }
        }
    }
}

void CSG::addObject(std::shared_ptr<GeometricObject> obj)
{
    tree.push_back(Data(obj));
}
void CSG::addOperation(OpType type)
{
    tree.push_back(Data(type));
}
void CSG::build()
{
    UpdateBoundingBox();
}
void CSG::SetMaterial(std::shared_ptr<Material> newMaterialPtr)
{
    for (auto& data : tree)
        if (data.type == DataType::OBJECT)
            data.obj->SetMaterial(newMaterialPtr);
}
HitRecord CSG::Hit(const Ray& ray)
{
    std::stack<std::vector<HitRecord>> intvStack;
    for (auto& data : tree)
    {
        if (data.type == DataType::OBJECT)
        {
            intvStack.push(data.obj->getHitInterval(ray));
        }
        else if (data.type == DataType::OPERATION)
        {
            auto e2 = intvStack.top();
            intvStack.pop();
            auto e1 = intvStack.top();
            intvStack.pop();

            if (data.optype == OpType::UNION)
                intvStack.push(performUnion(e1, e2));
            else if (data.optype == OpType::INTERSECTION)
                intvStack.push(performIntection(e1, e2));
            else if (data.optype == OpType::DIFFERENCE)
                intvStack.push(performDifference(e1, e2));
        }
    }
    for (auto& record : intvStack.top())
    {
        if (record.T > kEpsilon)
            return record;
    }
    return HitRecord();
}
