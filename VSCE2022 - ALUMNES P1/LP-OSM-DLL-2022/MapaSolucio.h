#pragma once
#include "pch.h"
#include "MapaBase.h"
#include "CamiSolucio.h"
#include "BallTree.h"
#include "GrafSolucio.h"
using namespace std;

class MapaSolucio: public MapaBase
{
public:
	MapaSolucio()
	{

	}
	~MapaSolucio()
	{

	}

	void getPdis(vector<PuntDeInteresBase*>& pdis) override;
	void getCamins(vector<CamiBase*>& pi) override;
	void parsejaXmlElements(vector<XmlElement>& xmlElements) override;

	CamiBase* buscaCamiMesCurt(PuntDeInteresBase* desde, PuntDeInteresBase* a);

private:
	vector<PuntDeInteresBase*> m_puntsInteres;
	vector<CamiBase*> m_camins;
	GrafSolucio m_graf;
	BallTree m_ballTree;

	int minDistance(const vector<double>& dist, const vector<bool>& visitat) const;

};