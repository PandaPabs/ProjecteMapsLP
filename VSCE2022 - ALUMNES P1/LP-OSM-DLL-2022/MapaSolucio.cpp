#include "pch.h"
#include "MapaSolucio.h"
#include "PuntDeInteresBotigaSolucio.h"
#include "PuntDeInteresRestaurantSolucio.h"

void MapaSolucio::getPdis(std::vector<PuntDeInteresBase*>& pdis) {
	Coordinate c;
	c.lat = 41.4918606;
	c.lon = 2.1465411;
	PuntDeInteresBotigaSolucio* b = new PuntDeInteresBotigaSolucio("La Millor Pastisseria", c , "bakery","",false);
	pdis.push_back(b);
	c = { 41.4902204, 2.1406477 };
	PuntDeInteresRestaurantSolucio* r = new PuntDeInteresRestaurantSolucio("El Millor Restaurant", c, true, "regional");
	pdis.push_back(r);
}