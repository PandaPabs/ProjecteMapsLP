#pragma once
#include "pch.h"
#include "MapaBase.h"

class MapaSolucio: public MapaBase
{
public:
	MapaSolucio() {}
	~MapaSolucio() {}

	void getPdis(std::vector<PuntDeInteresBase*>& pdis) override;
	virtual void getCamins(std::vector<CamiBase*>&) override {}
	virtual void parsejaXmlElements(std::vector<XmlElement>& xmlElements) override {}

private:

};