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
            GridIndex(size_t i, size_t j, size_t k);
            bool operator==(const GridIndex& other) const;
		};
		struct GridIndexHasher
		{
            std::size_t operator()(const GridIndex& key) const;
		};
		std::unordered_map<GridIndex, std::shared_ptr<GeometricObject>, GridIndexHasher> cells;
		int nx = 1, ny = 1, nz = 1;
	public:
        virtual ~Grid() = default;
        void Setup();
        HitRecord Hit(const Ray& ray) override;
	};
}
