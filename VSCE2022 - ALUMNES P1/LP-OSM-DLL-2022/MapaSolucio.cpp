#include "pch.h"
#include "CamiSolucio.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

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
	for (int element = 0; element < xmlElements.size(); element++)
	{
		if(xmlElements[element].id_element == "node")
		{
			int indexTipusPI = 0;
			while (xmlElements[element].fills[indexTipusPI].second[0].second != "shop" && xmlElements[element].fills[indexTipusPI].second[1].second != "restaurant" /* && xmlElements[element].fills[indexTipusPI].second[0].second != "highway" && xmlElements[element].fills[indexTipusPI].second[0].second != "public_transport" && xmlElements[element].fills[indexTipusPI].second[0].second != "entrance" && xmlElements[element].fills[indexTipusPI].second[0].second != "access"*/)
			{
				indexTipusPI++;
			}

			double lat;
			double lon;
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
			}
			Coordinate coord = { lat, lon };

			string name = "";
			if (xmlElements[element].fills[indexTipusPI].second[0].second == "shop")
			{//Shop
				string shopType = "";
				string openingHours = "";
				bool movReduida = false;
				for (int i = 0; i < xmlElements[element].fills.size(); i++)
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
					else if(xmlElements[element].fills[i].second[0].second == "wheelchair")
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
				m_puntsInteres.push_back(new PuntDeInteresBotigaSolucio(name, coord, shopType, openingHours, movReduida));
			}
			else if(xmlElements[element].fills[indexTipusPI].second[0].second == "amenity" && xmlElements[element].fills[indexTipusPI].second[1].second == "restaurant")
			{//Restaurant
				bool movReduida = false;
				string cuisine = "";
				for (int i = 0; i < xmlElements[element].fills.size(); i++)
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
				m_puntsInteres.push_back(new PuntDeInteresRestaurantSolucio(name, coord, movReduida, cuisine));
			}
			else //if(xmlElements[element].fills.size() == 0 || xmlElements[element].fills[indexTipusPI].second[0].second == "highway" || xmlElements[element].fills[indexTipusPI].second[0].second == "public_transport" || xmlElements[element].fills[indexTipusPI].second[0].second == "entrance" || xmlElements[element].fills[indexTipusPI].second[0].second == "access")
			{//Cami
				m_camins.addCoordenades(coord);
			}
			
		}
		else if (xmlElements[element].id_element == "way")
		{

		}
	}
}