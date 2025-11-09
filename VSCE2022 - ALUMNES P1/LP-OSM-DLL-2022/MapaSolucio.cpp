#include "pch.h"
#include "CamiSolucio.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"
#include <list>

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis) {
	//Coordinate c;
	//c.lat = 41.4918606;
	//c.lon = 2.1465411;
	//PuntDeInteresBotigaSolucio* b = new PuntDeInteresBotigaSolucio("La Millor Pastisseria", c , "bakery","",false);
	//pdis.push_back(b);
	//c = { 41.4902204, 2.1406477 };
	//PuntDeInteresRestaurantSolucio* r = new PuntDeInteresRestaurantSolucio("El Millor Restaurant", c, true, "regional");
	//pdis.push_back(r);

	pdis = m_puntsInteres;
}

void MapaSolucio::getCamins(std::vector<CamiBase*>& pi) {
	//CamiSolucio* caminet = new CamiSolucio;
	//pi.push_back(caminet);

	pi = m_camins;
}

void MapaSolucio::parsejaXmlElements(std::vector<XmlElement>& xmlElements)
{
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
					if(xmlElements[element].fills[i].first != "#text")
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
				if(esCami == false)
				{
					esCami = !teNom;
				}
			}


			if (esCami == true)
			{
				auxNodesCami.push_back({id, coord});
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
					while(auxNodesCami[j].first != IDNodeCami)
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
			m_camins.push_back(auxCami);
		}
	}
}