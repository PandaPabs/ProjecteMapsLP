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
	void addCoordenades(Coordinate coord) { m_coordenades.push_back(coord); }
	
private:
	vector<Coordinate> m_coordenades;
};