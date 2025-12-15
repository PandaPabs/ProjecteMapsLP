#pragma once
#include "Util.h"
#include "CamiSolucio.h"

class GrafSolucio
{
public:
	GrafSolucio() {};

	void inicialitza(vector<CamiBase*> camins);

private:
	vector<vector<long>> m_matrAdj;
	vector<Coordinate> m_nodes; //Vector de coordenades
};