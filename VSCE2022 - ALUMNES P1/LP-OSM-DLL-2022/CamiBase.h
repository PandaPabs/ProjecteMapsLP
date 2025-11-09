#pragma once
#include "pch.h"

#include "Common.h"
#include <vector>

class CamiBase {
	public:
		virtual std::vector<Coordinate> getCamiCoords() = 0;
		virtual void addCoordenades(Coordinate coord) = 0;
		
		
};

