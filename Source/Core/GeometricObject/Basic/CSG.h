/*
  ==============================================================================

    CSG.h
    Created: 17 Nov 2018 12:21:46am
    Author:  bowen

  ==============================================================================
*/

#pragma once

#include <stack>
#include "../GeometricObject.h"

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

    std::vector<std::pair<HitRecord, bool>> mergeIntervals(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2);
    std::vector<HitRecord> performUnion(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2);
    std::vector<HitRecord> performIntection(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2);
    std::vector<HitRecord> performDifference(std::vector<HitRecord>& e1, std::vector<HitRecord>& e2);
protected:
    void UpdateBoundingBox() override;

public:
    virtual ~CSG() = default;
    void addObject(std::shared_ptr<GeometricObject> obj);
    void addOperation(OpType type);
    void build();
    void SetMaterial(std::shared_ptr<Material> newMaterialPtr) override;
    virtual HitRecord Hit(const Ray& ray) override;

    static std::shared_ptr<GeometricObject> parse(StringArray& cmd, std::unordered_map<String, std::shared_ptr<void>>& env);
};
