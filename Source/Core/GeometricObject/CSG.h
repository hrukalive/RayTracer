/*
  ==============================================================================

    CSG.h
    Created: 17 Nov 2018 12:21:46am
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include <stack>
#include "../Utility.h"
#include "GeometricObject.h"

class CSG : public GeometricObject
{
public:
    enum OpType
    {
        UNION,
        INTERSECTION,
        DIFFERENCE
    };
private:
    enum DataType
    {
        OBJECT,
        OPERATION
    };
    struct Data
    {
        DataType type;
        OpType optype;
        std::shared_ptr<GeometricObject> obj;
        Data(OpType optype) : type(DataType::OPERATION), optype(optype), obj(nullptr) {}
        Data(std::shared_ptr<GeometricObject> obj) : type(DataType::OBJECT), obj(obj) {}
    };
    std::vector<Data> tree;

    std::vector<std::pair<HitRecord, bool>> mergeIntervals(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
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
    std::vector<HitRecord> performUnion(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
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
    std::vector<HitRecord> performIntection(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
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
    std::vector<HitRecord> performDifference(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2)
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
protected:
    void UpdateBoundingBox() override
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
public:
    virtual ~CSG() = default;
    void addObject(std::shared_ptr<GeometricObject> obj)
    {
        tree.push_back(Data(obj));
    }
    void addOperation(OpType type)
    {
        tree.push_back(Data(type));
    }
    void build()
    {
        UpdateBoundingBox();
    }
    void SetMaterial(std::shared_ptr<Material> newMaterialPtr)
    {
        for (auto& data : tree)
            if (data.type == DataType::OBJECT)
                data.obj->SetMaterial(newMaterialPtr);
    }
    virtual HitRecord Hit(const Ray& ray) override
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
                else if (data.optype ==  OpType::DIFFERENCE)
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
};
