/*
  ==============================================================================

    Grid.h
    Created: 4 Oct 2018 4:04:30pm
    Author:  Bowen Sun

  ==============================================================================
*/

#pragma once
#include "Compound.h"
#include <unordered_map>

namespace RayTracer
{
	class Grid : public Compound
	{
		struct GridIndex
		{
			size_t i, j, k;
			GridIndex(size_t i, size_t j, size_t k) : i(i), j(j), k(k) {}
			bool operator==(const GridIndex& other) const
			{
				return i == other.i && j == other.j && k == other.k;
			}
		};
		struct GridIndexHasher
		{
			std::size_t operator()(const GridIndex& key) const
			{
				return m3D_e_magicbits(key.i, key.j, key.k);
			}
		};
		std::unordered_map<GridIndex, std::shared_ptr<GeometricObject>, GridIndexHasher> cells;
		int nx = 1, ny = 1, nz = 1;
		inline long clamp(FP_TYPE x, long min, long max) { return long((x < min ? min : (x > max ? max : x))); }
	public:
        virtual ~Grid();
        void Setup();
        HitRecord Hit(const Ray& ray) override;
	};
}
