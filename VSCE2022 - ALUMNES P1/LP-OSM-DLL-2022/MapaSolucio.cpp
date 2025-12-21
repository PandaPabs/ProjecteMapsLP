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
	std::vector<int> debug;
	std::vector<pair<string, Coordinate>> auxNodesCami;
	for (int element = 0; element < xmlElements.size(); element++)
	{
		if (xmlElements[element].id_element == "node")
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
					if (id == "6757996103") //Para depurar
					{
						int z = 0;
					}
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
					if (xmlElements[element].fills[i].first != "#text")
					{
						if (xmlElements[element].fills[i].second[0].second == "highway" || xmlElements[element].fills[i].second[0].second == "public_transport" || xmlElements[element].fills[i].second[0].second == "entrance" || xmlElements[element].fills[i].second[0].second == "access")
						{
							esCami = true;
						}
						if (xmlElements[element].fills[i].second[0].second == "name")
						{
							teNom = true;
						}
					}
				}
				if (esCami == false)
				{
					esCami = !teNom;
				}
			}

			auxNodesCami.push_back({ id, coord });

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
						if (xmlElements[element].fills[indexTipusPI].second[0].second == "shop" || xmlElements[element].fills[indexTipusPI].second[0].second == "cuisine")
						{
							break;
						}
					}
					indexTipusPI++;
				}

				string name = "";
				if (xmlElements[element].fills.size() != indexTipusPI)
				{
					if (xmlElements[element].fills[indexTipusPI].second[0].second == "shop")
					{//Shop
						string shopType = "";
						string openingHours = "";
						bool movReduida = false;
						for (int i = 0; i < xmlElements[element].fills.size(); i++)
						{
							if (xmlElements[element].fills[i].first != "#text")
							{
								if (xmlElements[element].fills[i].second[0].second == "name")
								{
									name = xmlElements[element].fills[i].second[1].second;
								}
								else if (xmlElements[element].fills[i].second[0].second == "shop")
								{
									shopType = xmlElements[element].fills[i].second[1].second;
								}
								else if (xmlElements[element].fills[i].second[0].second == "opening_hours")
								{
									openingHours = xmlElements[element].fills[i].second[1].second;
								}
								else if (xmlElements[element].fills[i].second[0].second == "wheelchair")
								{
									if (xmlElements[element].fills[i].second[1].second == "yes")
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
								if (xmlElements[element].fills[i].second[0].second == "name")
								{
									name = xmlElements[element].fills[i].second[1].second;
								}
								else if (xmlElements[element].fills[i].second[0].second == "wheelchair")
								{
									if (xmlElements[element].fills[i].second[1].second == "yes")
									{
										movReduida = true;
									}
									else
									{
										movReduida = false;
									}
								}
								else if (xmlElements[element].fills[i].second[0].second == "cuisine")
								{
									cuisine = xmlElements[element].fills[i].second[1].second;
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
							if (xmlElements[element].fills[i].second[0].second == "name")
							{
								name = xmlElements[element].fills[i].second[1].second;
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
			if (element == 3568) //Para depurar
			{
				int a = 0;
			}
			CamiBase* auxCami = new CamiSolucio();
			for (int i = 0; i < xmlElements[element].fills.size(); i++)
			{
				if (i == 7) //Para depurar
				{
					int b = 0;
				}
				if (xmlElements[element].fills[i].first == "nd")
				{
					string IDNodeCami = xmlElements[element].fills[i].second[0].second;
					int j = 0;
					while (auxNodesCami[j].first != IDNodeCami)
					{
						j++;
						if (j == 2378) //Para depurar
						{
							int c = 0;
						}
					}
					auxCami->addCoordenades(auxNodesCami[j].second);
				}
			}
			if (auxCami->getCamiCoords().size() >= 2) {
				m_camins.push_back(auxCami);
			}
			else {
				delete auxCami;
			}
		}
	}

	m_graf.inicialitza(m_camins);

	//peta?

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
	for (int i = 0; i < dist.size(); i++) {
		if (!visitat[i] && dist[i] < min) {
			minIndex = i;
			min = dist[i];
		}
	}
	return minIndex;
}


CamiBase* MapaSolucio::buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a) {
	Util::escriuEnMonitor("He entrado en me como mi caca");

	Coordinate Qin;
	vector<list<Coordinate>> recorrido;
	m_ballTree.preOrdre(recorrido);

	if (!recorrido.empty() && !recorrido[0].empty()) {
		Qin = recorrido[0].front();
	}
	else {
		Qin = { 0.0,0.0 };
	}
	Coordinate cOrigen = m_ballTree.nodeMesProper(desde->getCoord(), Qin, m_ballTree.getArrel());
	
	Qin = cOrigen;
	Coordinate cDesti = m_ballTree.nodeMesProper(a->getCoord(), Qin, m_ballTree.getArrel());
	

	Util::escriuEnMonitor("Punto origen: " + desde->getName());
	Util::escriuEnMonitor("Punto destino: " + a->getName());
	Util::escriuEnMonitor("Nodo camino origen encontrado: " + to_string(cOrigen.lat) + ", " + to_string(cOrigen.lon));
	Util::escriuEnMonitor("Nodo camino destino encontrado: " + to_string(cDesti.lat) + ", " + to_string(cDesti.lon));

	int nodoOrigen = m_graf.getNodeId(cOrigen);
	int nodoDestino = m_graf.getNodeId(cDesti);


	//if nodes no valids
	if (nodoOrigen == -1 || nodoDestino == -1) {
		Util::escriuEnMonitor("Error: nodos origen o destino no encontrados en el grafo");
		return new CamiSolucio();

	}
	if (nodoOrigen >= m_graf.getNumNodes() || nodoDestino >= m_graf.getNumNodes()) {
		Util::escriuEnMonitor("Error: índices fuera de rango");
		return new CamiSolucio();
	}

	//dikjstra
	int n = m_graf.getNumNodes();

	vector<double> distancias(n, DBL_MAX);
	vector<int> anterior(n, -1);
	vector<bool> visitados(n, false);

	distancias[nodoOrigen] = 0.0;
	anterior[nodoOrigen] = -1;
	// Algoritmo Dijkstra
	for (int count = 0; count < n - 1; count++) {
		int u = minDistance(distancias, visitados);

		if (u == -1) {
			break;
		}

		visitados[u] = true;

		for (int v = 0; v < n; v++) {
			
			double dist = m_graf.getDistancia(u, v);

			if (dist < DBL_MAX - 1.0) {
				if (!visitados[v]) {
					double nuevaDist = distancias[u] + dist;
					if (nuevaDist < distancias[v]) {
						distancias[v] = nuevaDist;
						anterior[v] = u;
					}
				}
			}
		}
	}

	CamiSolucio* cami = new CamiSolucio();

	if (distancias[nodoDestino] >= DBL_MAX - 1) {
		Util::escriuEnMonitor("No se encontró camino entre los puntos");
		return new CamiSolucio();
	}


	vector<int> camiIds;
	for (int v = nodoDestino; v != -1; v = anterior[v]) {
		camiIds.push_back(v);
	}


	reverse(camiIds.begin(), camiIds.end());

	for (int id : camiIds) {
		cami->addCoordenades(m_graf.getNode(id));
	}

	Util::escriuEnMonitor("Camino encontrado con " + to_string(camiIds.size()) + " nodos");
	return cami;
}