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
	int nNodes = m_nodes.size();
	m_matrAdj.resize(nNodes); // fer resize files

	for (int i = 0; i < nNodes; i++) 
	{
		m_matrAdj[i].resize(nNodes, DBL_MAX); // resize columnes
	}

	m_matrAdj[nNodes - 1][nNodes - 1] = 0; // el nou node te una distancia de 0 amb ell mateix

	return nNodes - 1; // retorna el nou index
}

void GrafSolucio::afegirAresta(int node1, int node2) 
{
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
			//afegirNode solo para recivir su indice
			int nod1 = afegirNode(coords[j]);
			int nod2 = afegirNode(coords[j + 1]);

			afegirAresta(nod1, nod2);
		}
	}
}