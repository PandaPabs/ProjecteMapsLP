#pragma once
#include "pch.h"
#include "CamiBase.h"
using namespace std;

class CamiSolucio: public CamiBase
{
public:
	CamiSolucio() {};
	~CamiSolucio() {};

	vector<Coordinate> getCamiCoords() override { return m_coordenades; };
	
private:
	vector<Coordinate> m_coordenades;
};