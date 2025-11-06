#pragma once
#include "pch.h"
#include "PuntDeInteresBase.h"
using namespace std;
class PuntDeInteresBotigaSolucio : public PuntDeInteresBase
{
public:
	PuntDeInteresBotigaSolucio() : m_shop(""), m_opHour(""), m_movReduida(false) {}
	PuntDeInteresBotigaSolucio(const string& name, const Coordinate& c,
		const string& shop, const string& op, bool mov) : PuntDeInteresBase(c, name), m_shop(shop), m_opHour(op), m_movReduida(mov) {
	}
	~PuntDeInteresBotigaSolucio() {}

	string getName() override;
	unsigned int getColor() override;

private:
	string m_shop;
	string m_opHour;
	bool m_movReduida;
};