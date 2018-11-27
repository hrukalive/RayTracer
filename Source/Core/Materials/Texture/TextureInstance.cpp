#include "Texture.h"

TextureInstance::TextureInstance(const std::shared_ptr<Texture> texture) : texturePtr(texture) {}

void TextureInstance::setInvMatrix(const Matrix& inv)
{
    invMatrix = inv;
}

void TextureInstance::Translate(const FP_TYPE dx, const FP_TYPE dy, const FP_TYPE dz)
{
    auto im = Matrix::identity(4);
    im(0, 3) = -dx;
    im(1, 3) = -dy;
    im(2, 3) = -dz;
    invMatrix = invMatrix * im;
}

void TextureInstance::Scale(const FP_TYPE a, const FP_TYPE b, const FP_TYPE c)
{
    auto im = Matrix::identity(4);
    im(0, 0) = 1.0 / a;
    im(1, 1) = 1.0 / b;
    im(2, 2) = 1.0 / c;
    invMatrix = invMatrix * im;
}

void TextureInstance::RotateX(const FP_TYPE radian)
{
    auto im = Matrix::identity(4);
    im(1, 1) = cos(radian);
    im(2, 2) = cos(radian);
    im(1, 2) = sin(radian);
    im(2, 1) = -sin(radian);
    invMatrix = invMatrix * im;
}

void TextureInstance::RotateY(const FP_TYPE radian)
{
    auto im = Matrix::identity(4);
    im(0, 0) = cos(radian);
    im(2, 2) = cos(radian);
    im(0, 2) = -sin(radian);
    im(2, 0) = sin(radian);
    invMatrix = invMatrix * im;
}

void TextureInstance::RotateZ(const FP_TYPE radian)
{
    auto im = Matrix::identity(4);
    im(0, 0) = cos(radian);
    im(1, 1) = cos(radian);
    im(0, 1) = sin(radian);
    im(1, 0) = -sin(radian);
    invMatrix = invMatrix * im;
}

inline RGBColor TextureInstance::getColor(const HitRecord& record) const
{
    HitRecord tmp = record;
    tmp.HitPoint = MatrixMulVector(invMatrix, tmp.HitPoint);
    return texturePtr->getColor(tmp);
}