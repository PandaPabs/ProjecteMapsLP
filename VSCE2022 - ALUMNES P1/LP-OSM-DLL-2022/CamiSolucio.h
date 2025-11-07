#pragma once
#include "pch.h"
#include "CamiBase.h"

class CamiSolucio: public CamiBase
{
public:
	CamiSolucio() {};
	~CamiSolucio() {};

	std::vector<Coordinate> getCamiCoords() override;
	
private:
	
};