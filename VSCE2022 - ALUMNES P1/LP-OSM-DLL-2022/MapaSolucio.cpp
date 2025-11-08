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
			while (xmlElements[element].fills[indexTipusPI].second[0].second != "shop" && xmlElements[element].fills[indexTipusPI].second[0].second != "cuisine")
			{
				indexTipusPI++;
			}

			if (xmlElements[element].fills[indexTipusPI].second[0].second == "shop")
			{
				PuntDeInteresBotigaSolucio* shop = new PuntDeInteresBotigaSolucio();
				for (int i = 0; i < xmlElements[element].fills.size(); i++)
				{
					if (xmlElements[element].fills[i].second[0].second == "wheelchair")
					{
						if (xmlElements[element].fills[i].second[1].second == "yes")
						{
							shop->setMovReduida(true);
						}
						else
						{
							shop->setMovReduida(false);
						}
					}
					//El resto de tipos de hijo
					
				}
				m_puntsInteres.push_back(shop);
			}
			else if(xmlElements[element].fills[indexTipusPI].second[0].second == "cuisine")
			{

				//Al final hay que tener un constructor por parametro de Restaurant.
			}
			else
			{
				//COMO IDENTIFICO UN CAMI?
			}
			
		}
		else if (xmlElements[element].id_element == "way")
		{

		}
	}
}