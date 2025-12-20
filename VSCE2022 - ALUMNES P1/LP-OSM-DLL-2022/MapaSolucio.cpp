#include "pch.h"
#include "CamiSolucio.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include <list>

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis) {
	pdis = m_puntsInteres;
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& pi) {
	pi = m_camins;
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
	m_camins.resize(0);
	m_puntsInteres.resize(0);
	Util util;
	std::vector<int> debug;
	std::vector<pair<string, Coordinate>> auxNodesCami;
	for (int element = 0; element < xmlElements.size(); element++)
	{
		if(xmlElements[element].id_element == "node")
		{
			//Calculo las Coordenadas
			double lat;
			double lon;
			string id;
			for (int i = 0; i < xmlElements[element].atributs.size(); i++)
			{
				if (xmlElements[element].atributs[i].first == "lat")
				{
					lat = std::stod(xmlElements[element].atributs[i].second);
				}
				else if (xmlElements[element].atributs[i].first == "lon")
				{
					lon = std::stod(xmlElements[element].atributs[i].second);
				}
				else if (xmlElements[element].atributs[i].first == "id")
				{
					id = xmlElements[element].atributs[i].second;
				}
			}
			Coordinate coord = { lat, lon };

			//Miro si es un Camino
			bool esCami = false;
			
			if (xmlElements[element].fills.size() == 0)
			{
				esCami = true;
			}
			else
			{
				bool teNom = false;
				for (int i = 0; i < xmlElements[element].fills.size(); i++)
				{
					if(xmlElements[element].fills[i].first != "#text")
					{
						auto kdt = util.kvDeTag(xmlElements[element].fills[i].second);
						string key = kdt.first;
						if (key == "highway" || key == "public_transport" || key == "entrance" || key == "access")
						{
							esCami = true;
						}
						if (key == "name")
						{
							teNom = true;
						}
					}
				}
				if(esCami == false)
				{
					esCami = !teNom;
				}
			}

			auxNodesCami.push_back({id, coord});

			if (esCami == true)
			{
			}
			else
			{
				int indexTipusPI = 0;
				while (xmlElements[element].fills.size() != indexTipusPI)
				{
					if (xmlElements[element].fills[indexTipusPI].first != "#text")
					{
						auto kdt = util.kvDeTag(xmlElements[element].fills[indexTipusPI].second);
						string key = kdt.first;
						if (key == "shop" || key == "cuisine")
						{
							break;
						}
					}
					indexTipusPI++;
				}

				string name = "";
				if (xmlElements[element].fills.size() != indexTipusPI)
				{
					auto kdt = util.kvDeTag(xmlElements[element].fills[indexTipusPI].second);
					string key = kdt.first;
					if (key == "shop")
					{//shop
						string shopType = "";
						string openingHours = "";
						bool movReduida = false;
						for (int i = 0; i < xmlElements[element].fills.size(); i++)
						{
							
							if (xmlElements[element].fills[i].first != "#text")
							{
								auto kvTag = util.kvDeTag(xmlElements[element].fills[i].second);
								string key = kvTag.first;
								string value = kvTag.second;
								if (key == "name")
								{
									name = value;
								}
								else if (key == "shop")
								{
									shopType = value;
								}
								else if (key == "opening_hours")
								{
									openingHours = value;
								}
								else if (key == "wheelchair")
								{
									if (value == "yes")
									{
										movReduida = true;
									}
									else
									{
										movReduida = false;
									}
								}
							}
						}
						m_puntsInteres.push_back(new PuntDeInteresBotigaSolucio(name, coord, shopType, openingHours, movReduida));
					}
					else if (/*xmlElements[element].fills[indexTipusPI].second[0].second == "amenity" && xmlElements[element].fills[indexTipusPI].second[1].second == "restaurant"*/ xmlElements[element].fills[indexTipusPI].second[0].second == "cuisine")
					{//Restaurant
						bool movReduida = false;
						string cuisine = "";
						for (int i = 0; i < xmlElements[element].fills.size(); i++)
						{
							if (xmlElements[element].fills[i].first != "#text")
							{
								auto kvTag = util.kvDeTag(xmlElements[element].fills[i].second);
								string key = kvTag.first;
								string value = kvTag.second;
								if (key == "name")
								{
									name = value;
								}
								else if (key == "wheelchair")
								{
									if (value == "yes")
									{
										movReduida = true;
									}
									else
									{
										movReduida = false;
									}
								}
								else if (key == "cuisine")
								{
									cuisine = value;
								}
							}
						}
						m_puntsInteres.push_back(new PuntDeInteresRestaurantSolucio(name, coord, movReduida, cuisine));
					}
				}
				else
				{
					for (int i = 0; i < xmlElements[element].fills.size(); i++)
					{
						if (xmlElements[element].fills[i].first != "#text")
						{
							auto kvTag = util.kvDeTag(xmlElements[element].fills[i].second);
							string key = kvTag.first;
							string value = kvTag.second;
							if (key == "name")
							{
								name = value;
							}
						}
					}
					m_puntsInteres.push_back(new PuntDeInteresBase(coord, name));
					debug.push_back(element); //Para saber donde estan los elementos que no acaban en la lista
				}
			}
		}
		else if (xmlElements[element].id_element == "way")
		{
			bool tieneHighway = false;
			for (int i = 0; i < xmlElements[element].fills.size(); i++)
			{
				if (xmlElements[element].fills[i].first == "tag")
				{
					auto kv = util.kvDeTag(xmlElements[element].fills[i].second);
					string key = kv.first;
					if (key == "highway") 
					{
						tieneHighway = true;
						break;
					}
				}
			}
			if (tieneHighway) 
			{
				CamiBase* auxCami = new CamiSolucio();
				for (int i = 0; i < xmlElements[element].fills.size(); i++)
				{
					if (xmlElements[element].fills[i].first == "nd")
					{
						string IDNodeCami = xmlElements[element].fills[i].second[0].second;
						int j = 0;
						while (auxNodesCami[j].first != IDNodeCami)
						{
							j++;
						}
						auxCami->addCoordenades(auxNodesCami[j].second);
					}
				}
				m_camins.push_back(auxCami);
			}
			
		}
	}

	m_graf.inicialitza(m_camins);

	vector<Coordinate> coords;
	for (CamiBase* cami : m_camins) {
		vector<Coordinate> c = cami->getCamiCoords();
		coords.insert(coords.end(), c.begin(), c.end());
	}
	m_ballTree.construirArbre(coords);
}



int MapaSolucio::minDistance(const vector<double>& dist, const vector<bool>& visitat) const
{
	// Initialize min value
	double min = DBL_MAX;
	int minIndex = -1;

	//OMPLIR
	for (int i = 0; i < m_graf.getNumNodes(); i++) {
		if (dist[i] < min && !visitat[i]) {
			minIndex = i;
			min = dist[i];
		}
	}
	return minIndex;
}


CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) {

	Coordinate Qin = {10000.0, 10000.0};
	Coordinate cOrigen = m_ballTree.nodeMesProper(desde->getCoord(), Qin, m_ballTree.getArrel());
	Qin = { 10000.0, 10000.0 };
	Coordinate cDesti = m_ballTree.nodeMesProper(a->getCoord(), Qin, m_ballTree.getArrel());
	int org = m_graf.getNodeId(cOrigen);
	int dest = m_graf.getNodeId(cDesti);
	
	//if nodes no valids
	if (org == -1 || dest == -1)
		return new CamiSolucio();
	//dikjstra
	
	vector<double> dist(m_graf.getNumNodes(), DBL_MAX);
	vector<int> anterior(m_graf.getNumNodes(), -1);
	vector<bool> visitats(m_graf.getNumNodes(), false);
	
	dist[org] = 0;
	
	double distancia;
	int minIndex = org;
	
	while (minIndex != -1 && !visitats[dest]) {
		visitats[minIndex] = true;
		distancia = dist[minIndex];

		if (distancia != DBL_MAX) {
			for (int i = 0; i < m_graf.getNumNodes(); i++) {
				if (i != minIndex && !visitats[i] && m_graf.getDistancia(minIndex, i) != DBL_MAX && distancia + m_graf.getDistancia(minIndex, i) < dist[i]) {
					dist[i] = distancia + m_graf.getDistancia(minIndex, i);
					anterior[i] = minIndex;
				}
			}
		}
		
		minIndex = minDistance(dist, visitats);
	}
	

	CamiSolucio* cami = new CamiSolucio();

	if (dist[dest] == DBL_MAX)
		return cami;

	vector<int> camiIds;
	for (int v = dest; v != -1; v = anterior[v])
		camiIds.push_back(v);

	reverse(camiIds.begin(), camiIds.end());

	for (int id : camiIds)
		cami->addCoordenades(m_graf.getNode(id));
	return cami;
}