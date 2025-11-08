#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "CamiBase.h"
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
	vector<CamiBase*> m_camins;
	vector<vector<CamiBase*>> m_ways; //No se si se hace así o con Coordenadas
};