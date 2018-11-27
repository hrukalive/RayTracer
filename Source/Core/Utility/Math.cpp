#include "Utility.h"

Vec3D ElemMul(const Vec3D& a, const Vec3D& b)
{
    return Vec3D(a.x * b.x, a.y * b.y, a.z * b.z);
}

Point3D MatrixMulPoint(const Matrix& m, const Point3D& p)
{
    Matrix pm = Matrix(4, 1);
    pm(0, 0) = p.x;
    pm(1, 0) = p.y;
    pm(2, 0) = p.z;
    pm(3, 0) = 1.0;
    auto res = m * pm;
    return Point3D(res(0, 0) / res(3, 0), res(1, 0) / res(3, 0), res(2, 0) / res(3, 0));
}

Point3D MatrixMulVector(const Matrix& m, const Vec3D& n)
{
    Matrix nm = Matrix(4, 1);
    nm(0, 0) = n.x;
    nm(1, 0) = n.y;
    nm(2, 0) = n.z;
    nm(3, 0) = 0.0;
    auto res = m * nm;
    return Point3D(res(0, 0), res(1, 0), res(2, 0));
}

Matrix MatrixTranspose(Matrix& m)
{
    Matrix ret(m);
    std::swap(ret(0, 1), ret(1, 0));
    std::swap(ret(0, 2), ret(2, 0));
    std::swap(ret(0, 3), ret(3, 0));
    std::swap(ret(1, 2), ret(2, 1));
    std::swap(ret(1, 3), ret(3, 1));
    std::swap(ret(2, 3), ret(3, 2));
    return ret;
}
