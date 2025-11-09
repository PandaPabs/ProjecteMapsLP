#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "CamiSolucio.h"
using namespace std;

class MapaSolucio: public MapaBase
{
public:
	MapaSolucio() {}
	~MapaSolucio() {}

	void getPdis(vector<PuntDeInteresBase*>& pdis) override;
	void getCamins(vector<CamiBase*>& pi) override;
	void parsejaXmlElements(vector<XmlElement>& xmlElements) override;

private:
	vector<PuntDeInteresBase*> m_puntsInteres;
	//vector<CamiSolucio*> m_camins; //El esquema de la pag 14 dice que MapaSolucio contiene un CamiSolucio. Pero entonces no cuadra con la funcion getCamins() :(
	CamiSolucio m_camins; //El esquema de la pag 14 dice que MapaSolucio contiene un CamiSolucio. Pero entonces no cuadra con la funcion getCamins() :(
};