#pragma once
#include "Util.h"
#include "CamiSolucio.h"
#include <vector>



class GrafSolucio
{
public:
	GrafSolucio() {};
	int afegirNode(const Coordinate& coord);
	void afegirAresta(int node1, int node2);


	void inicialitza(vector<CamiBase*> camins);

	int getNumNodes() const { return m_nodes.size(); }
	Coordinate getNode(int index) const { return m_nodes[index]; }
	double getDistancia(int i, int j) const { return m_matrAdj[i][j]; }
	int getNodeId(const Coordinate& node);

private:

	vector<vector<double>> m_matrAdj;
	vector<Coordinate> m_nodes; 
	
};