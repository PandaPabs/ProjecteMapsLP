#include "pch.h"
#include "GrafSolucio.h"

void GrafSolucio::inicialitza(vector<CamiBase*> camins)
{
	//inicialitza m_nodes
	for (int i = 0; i < camins.size(); i++)
	{
		vector<Coordinate> auxCoords = camins[i]->getCamiCoords();
		for (int j = 0; j < auxCoords.size(); j++)
		{
			int k = 0;
			while (k < m_nodes.size() && !(m_nodes[k].lat == auxCoords[j].lat && m_nodes[k].lon == auxCoords[j].lon))
			{
				k++;
			}

			if (k == m_nodes.size())
			{
				m_nodes.push_back(auxCoords[j]);
			}
		}
	}



	//inicialitza m_matrAdj
	Util util;
	long dist;
	m_matrAdj.resize(m_nodes.size());
	for (int i = 0; i < m_nodes.size(); i++)
	{
		m_matrAdj[i].resize(m_nodes.size(), LONG_MAX);
		for (int j = i + 1; j < m_nodes.size(); j++)
		{
			dist = util.DistanciaHaversine(m_nodes[i], m_nodes[j]);
			m_matrAdj[i][j] = dist;
			m_matrAdj[j][i] = dist;
		}
	}
}