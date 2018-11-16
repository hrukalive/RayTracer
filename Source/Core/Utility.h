/*
  ==============================================================================

    Utility.h
    Created: 28 Sep 2018 11:07:40am
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once

#include "../../JuceLibraryCode/JuceHeader.h"

#define FP_TYPE double
#define kEpsilon 0.0001
#define kHugeValue 1.0E10
#define kBBOXRatio 0.01
#define KBBOXMax 1.0
#define PI 3.1415926535897932384
#define TWO_PI 6.2831853071795864769
#define INV_PI 0.3183098861837906715
#define INV_TWO_PI 0.1591549430918953358
#define PI_OVER_180 0.0174532925199432957
#define NOISE_TABLE_SIZE 256
#define NOISE_TABLE_MASK (NOISE_TABLE_SIZE - 1)

typedef Vector3D<FP_TYPE> Vec3D;
typedef Vec3D Point3D;
typedef Vec3D RGBColor;
typedef Point<FP_TYPE> Point2D;
typedef dsp::Matrix<FP_TYPE> Matrix;

#define BLACK (RGBColor(0.0, 0.0, 0.0))
#define WHITE (RGBColor(1.0, 1.0, 1.0))
#define RED (RGBColor(1.0, 0.0, 0.0))

#define ZERO (Vec3D(0.0, 0.0, 0.0))
#define ONE (Vec3D(1.0, 1.0, 1.0))
#define X_DIR (Vec3D(1.0, 0.0, 0.0))
#define Y_DIR (Vec3D(0.0, 1.0, 0.0))
#define Z_DIR (Vec3D(0.0, 0.0, 1.0))

bool operator<(const Point3D& lhs, const Point3D& rhs);
std::ostream& operator<<(std::ostream& o, const Vec3D& vec);
std::ostream& operator<<(std::ostream& o, const Matrix& m);

std::vector<FP_TYPE> bilaterialFilter(const std::vector<FP_TYPE>& I, int width, int height);

template<class T>
inline T clamp(FP_TYPE x, T min, T max) { return T((x < min ? min : (x > max ? max : x))); }

inline Vec3D ElemMul(const Vec3D& a, const Vec3D& b)
{
	return Vec3D(a.x * b.x, a.y * b.y, a.z * b.z);
}

inline Point3D MatrixMulPoint(const Matrix& m, const Point3D& p)
{
	Matrix pm = Matrix(4, 1);
	pm(0, 0) = p.x;
	pm(1, 0) = p.y;
	pm(2, 0) = p.z;
	pm(3, 0) = 1.0;
	auto res = m * pm;
	return Point3D(res(0, 0) / res(3, 0), res(1, 0) / res(3, 0), res(2, 0) / res(3, 0));
}

inline Point3D MatrixMulVector(const Matrix& m, const Vec3D& n)
{
	Matrix nm = Matrix(4, 1);
	nm(0, 0) = n.x;
	nm(1, 0) = n.y;
	nm(2, 0) = n.z;
	nm(3, 0) = 0.0;
	auto res = m * nm;
	return Point3D(res(0, 0), res(1, 0), res(2, 0));
}

inline Matrix MatrixTranspose(Matrix& m)
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

template<class T>
inline T lerp(const FP_TYPE f, const T& a, const T& b)
{
    return a + (b - a) * f;
}

template<class T>
inline T cubicSpline(const FP_TYPE x, const T p0, const T p1, const T p2, const T p3)
{
    T c1 = 0.5 * (-p0 + p2);
    T c2 = p0 - 2.5 * p1 + 2.0 * p2 - 0.5 * p3;
    T c3 = -0.5 * p0 + 1.5 * p1 - 1.5 * p2 + 0.5 * p3;
    return (T)((((c3 * x) + c2) * x + c1) * x + p1);
}

// https://github.com/Forceflow/libmorton/blob/master/libmorton/include/morton3D.h
// Magicbits masks (3D encode)
static uint_fast32_t magicbit3D_masks32_encode[6] = { 0x000003ff, 0, 0x30000ff, 0x0300f00f, 0x30c30c3, 0x9249249 }; // we add a 0 on position 1 in this array to use same code for 32-bit and 64-bit cases
static uint_fast64_t magicbit3D_masks64_encode[6] = { 0x1fffff, 0x1f00000000ffff, 0x1f0000ff0000ff, 0x100f00f00f00f00f, 0x10c30c30c30c30c3, 0x1249249249249249 };

// HELPER METHOD: Magic bits encoding (helper method)
static inline size_t morton3D_SplitBy3bits(const size_t a) {
	const size_t* masks = (sizeof(size_t) <= 4) ? reinterpret_cast<const size_t*>(magicbit3D_masks32_encode) : reinterpret_cast<const size_t*>(magicbit3D_masks64_encode);
	size_t x = ((size_t)a) & masks[0];
	if (sizeof(size_t) == 8) { x = (x | (uint_fast64_t)x << 32) & masks[1]; } // for 64-bit case
	x = (x | x << 16) & masks[2];
	x = (x | x << 8)  & masks[3];
	x = (x | x << 4)  & masks[4];
	x = (x | x << 2)  & masks[5];
	return x;
}

// ENCODE 3D Morton code : Magic bits method
// This method uses certain bit patterns (magic bits) to split bits in the coordinates
inline size_t m3D_e_magicbits(const size_t x, const size_t y, const size_t z) {
	return morton3D_SplitBy3bits(x) | (morton3D_SplitBy3bits(y) << 1) | (morton3D_SplitBy3bits(z) << 2);
}

class Tracer;

struct Ray
{
	Point3D Origin;
	Vec3D Direction;
	Ray() {}
    Ray(Vec3D origin, Vec3D direction) : Origin(origin), Direction(direction) {}
	inline Point3D GetPoint(FP_TYPE t) const
	{
		return Origin + (Direction * t);
	}
};

class Material;
class World;
struct HitRecord
{
	bool Hit = false;
	Vec3D Normal;
    bool NormalFlipped = false;
	Vec3D HitPoint;
	FP_TYPE T = 0.0f;
    int Depth = 0;
	Ray Ray;
    std::shared_ptr<Material> MaterialPtr = nullptr;

	HitRecord() {}
	HitRecord(Vec3D normal, Vec3D hitPoint, FP_TYPE t)
		: Normal(normal), HitPoint(hitPoint), T(t) {}
};

struct ColorRamp
{
    template<class U, class V>
    struct pairLess
    {
        constexpr bool operator()(const std::pair<U, V> &lhs, const std::pair<U, V> &rhs) const
        {
            return lhs.first < rhs.first;
        }
    };
    std::vector<std::pair<int, RGBColor>> colorPoints;

    int N;
    std::vector<RGBColor> colors;
public:
    ColorRamp(int N = 100) : N(N) { colors = std::vector<RGBColor>(N + 1); }
    void addColorPoint(FP_TYPE x, RGBColor color)
    {
        int newX = clamp((int)(N * x), 0, N);
        colorPoints.push_back(std::make_pair(newX, color));
    }
    void build()
    {
        std::sort(colorPoints.begin(), colorPoints.end(), pairLess<int, RGBColor>());
        for (size_t i = 0; i < colorPoints.size() - 1; i++)
        {
            auto lowIdx = colorPoints[i].first;
            auto highIdx = colorPoints[i + 1].first;
            auto lowColor = colorPoints[i].second;
            auto highColor = colorPoints[i + 1].second;
            for (size_t j = colorPoints[i].first; j < colorPoints[i + 1].first; j++)
                colors[j] = lerp(double(j - lowIdx) / double(highIdx - lowIdx), lowColor, highColor);
        }
    }
    void buildFromImage(Image image, bool isHorizontal = true)
    {
        if (isHorizontal)
        {
            N = image.getWidth();
            colors = std::vector<RGBColor>(N + 1);
            for (int i = 0; i < N; i++)
            {
                auto c = image.getPixelAt(i, 0);
                colors[i] = RGBColor(c.getRed() / 255.0, c.getGreen() / 255.0, c.getBlue() / 255.0);
            }
        }
        else
        {
            N = image.getHeight();
            colors = std::vector<RGBColor>(N + 1);
            for (int i = 0; i < N; i++)
            {
                auto c = image.getPixelAt(0, i);
                colors[i] = RGBColor(c.getRed() / 255.0, c.getGreen() / 255.0, c.getBlue() / 255.0);
            }
        }
    }
    RGBColor getColor(FP_TYPE x)
    {
        auto fx = floor(x * N), cx = ceil(x * N);
        auto dx = x * N - fx;
        return lerp(dx, colors[fx], colors[cx]);
    }
};
