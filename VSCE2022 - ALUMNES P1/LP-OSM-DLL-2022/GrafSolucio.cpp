#include "pch.h"
#include "GrafSolucio.h"


int GrafSolucio::afegirNode(const Coordinate& coord) 
{
	for (int i = 0; i < m_nodes.size(); i++) //Mirar si ja està
	{
		if (m_nodes[i].lat == coord.lat && m_nodes[i].lon == coord.lon) 
		{
			return i; // si ja està retornes el seu index
		}
	}


	m_nodes.push_back(coord); // si no està l'afegeixes
	int nouIndex = m_nodes.size() - 1;

	int antic = m_matrAdj.size();
	int nou = antic + 1;

	for (int i = 0; i < antic; i++) {
		m_matrAdj[i].resize(nou, DBL_MAX);
	}

	m_matrAdj.resize(nou);
	m_matrAdj[nouIndex].resize(nou, DBL_MAX);

	for (int i = 0; i < nou; i++) {
		m_matrAdj[nouIndex][i] = DBL_MAX;
		m_matrAdj[i][nouIndex] = DBL_MAX;
	}

	m_matrAdj[nouIndex][nouIndex] = 0; // el nou node te una distancia de 0 amb ell mateix

	return nouIndex; // retorna el nou index
}

void GrafSolucio::afegirAresta(int node1, int node2) 
{
	if (node1 < 0 || node1 >= m_nodes.size() || node2 < 0 || node2 >= m_nodes.size())
		return;
	Util util;
	double d = util.DistanciaHaversine(m_nodes[node1], m_nodes[node2]);
	m_matrAdj[node1][node2] = d;
	m_matrAdj[node2][node1] = d;
}


int GrafSolucio::getNodeId(const Coordinate& node) {
	for (int i = 0; i < m_nodes.size(); i++) {
		if (node.lat == m_nodes[i].lat && node.lon == m_nodes[i].lon) {
			return i;
		}
	}
	return -1;
}

void GrafSolucio::inicialitza(vector<CamiBase*> camins)
{
	m_nodes.clear();
	m_matrAdj.clear();
	//inicialitza m_nodes
	for (int i = 0; i < camins.size(); i++)
	{
		vector<Coordinate> auxCoords = camins[i]->getCamiCoords();
		for (int j = 0; j < auxCoords.size(); j++)
		{
			afegirNode(auxCoords[j]);
		}
	}

	//inicialitza arestes
	for (int i = 0; i < camins.size(); i++) 
	{
		vector<Coordinate> coords = camins[i]->getCamiCoords();
		for (int j = 0; j < coords.size() - 1; j++) 
		{
			int nod1 = getNodeId(coords[j]);
			int nod2 = getNodeId(coords[j + 1]);

			if (nod1 != -1 && nod2 != -1 && nod1 != nod2)
				afegirAresta(nod1, nod2);
		}
	}
}