#include "Instance.h"

void Instance::Transform(const Matrix& transform, const Matrix& invTransform)
{
    forwardMatrix = transform * forwardMatrix;
    invMatrix = invMatrix * invTransform;
    invMatrixT = MatrixTranspose(invMatrix);

    UpdateBoundingBox();
}

void Instance::UpdateBoundingBox()
{
    if (!objectPtr)
        return;
    BBox& objBBox = objectPtr->GetBoundingBox();
    auto min = objBBox.GetMinPoint();
    auto max = objBBox.GetMaxPoint();
    auto x0 = min.x, y0 = min.y, z0 = min.z;
    auto x1 = max.x, y1 = max.y, z1 = max.z;
    auto tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y0, z0));
    boundingBox = BBox(tmp, tmp);
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y0, z0));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y1, z0));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y0, z1));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y1, z0));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x0, y1, z1));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y0, z1));
    boundingBox.Merge(BBox(tmp, tmp));
    tmp = MatrixMulPoint(forwardMatrix, Point3D(x1, y1, z1));
    boundingBox.Merge(BBox(tmp, tmp));
}

Instance::Instance(std::shared_ptr<GeometricObject> objectPtr) : objectPtr(objectPtr)
{
    UpdateBoundingBox();
}

void Instance::SetObject(std::shared_ptr<GeometricObject> newObjectPtr)
{
    objectPtr = newObjectPtr;
    UpdateBoundingBox();
}

void Instance::Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz)
{
    auto m = Matrix::identity(4);
    m(0, 3) = dx;
    m(1, 3) = dy;
    m(2, 3) = dz;
    auto im = Matrix::identity(4);
    im(0, 3) = -dx;
    im(1, 3) = -dy;
    im(2, 3) = -dz;
    Transform(m, im);
}

void Instance::Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c)
{
    auto m = Matrix::identity(4);
    m(0, 0) = a;
    m(1, 1) = b;
    m(2, 2) = c;
    auto im = Matrix::identity(4);
    im(0, 0) = 1.0 / a;
    im(1, 1) = 1.0 / b;
    im(2, 2) = 1.0 / c;
    Transform(m, im);
}

void Instance::RotateX(const FP_TYPE radian)
{
    auto m = Matrix::identity(4);
    m(1, 1) = cos(radian);
    m(2, 2) = cos(radian);
    m(1, 2) = -sin(radian);
    m(2, 1) = sin(radian);
    auto im = Matrix::identity(4);
    im(1, 1) = cos(radian);
    im(2, 2) = cos(radian);
    im(1, 2) = sin(radian);
    im(2, 1) = -sin(radian);
    Transform(m, im);
}

void Instance::RotateY(const FP_TYPE radian)
{
    auto m = Matrix::identity(4);
    m(0, 0) = cos(radian);
    m(2, 2) = cos(radian);
    m(0, 2) = sin(radian);
    m(2, 0) = -sin(radian);
    auto im = Matrix::identity(4);
    im(0, 0) = cos(radian);
    im(2, 2) = cos(radian);
    im(0, 2) = -sin(radian);
    im(2, 0) = sin(radian);
    Transform(m, im);
}

void Instance::RotateZ(const FP_TYPE radian)
{
    auto m = Matrix::identity(4);
    m(0, 0) = cos(radian);
    m(1, 1) = cos(radian);
    m(0, 1) = -sin(radian);
    m(1, 0) = sin(radian);
    auto im = Matrix::identity(4);
    im(0, 0) = cos(radian);
    im(1, 1) = cos(radian);
    im(0, 1) = sin(radian);
    im(1, 0) = -sin(radian);
    Transform(m, im);
}

HitRecord Instance::Hit(const Ray& ray)
{
    Ray invRay(MatrixMulPoint(invMatrix, ray.Origin), MatrixMulVector(invMatrix, ray.Direction));
    HitRecord record = objectPtr->Hit(invRay);
    record.Ray = ray;
    record.HitPoint = ray.GetPoint(record.T);
    record.Normal = MatrixMulVector(invMatrixT, record.Normal).normalised();
    if (materialPtr != nullptr)
        record.MaterialPtr = materialPtr;
    return record;
}
