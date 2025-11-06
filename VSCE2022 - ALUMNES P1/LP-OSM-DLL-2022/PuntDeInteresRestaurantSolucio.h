#pragma once
#include "pch.h"
#include "PuntDeInteresBase.h"
#include <string>

class PuntDeInteresRestaurantSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresRestaurantSolucio() : m_movReduida(false) {}
	~PuntDeInteresRestaurantSolucio() {}
	PuntDeInteresRestaurantSolucio(const std::string& name, const Coordinate& c,
		bool mov, const std::string& ca) : PuntDeInteresBase(c, name), m_movReduida(mov), m_cuisine(ca) {
	}

	std::string getName() override;
	unsigned int getColor() override;
private:
	bool m_movReduida;
	std::string m_cuisine;
};

