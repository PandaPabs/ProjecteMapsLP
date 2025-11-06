#include "pch.h"
#include "PuntDeInteresBotigaSolucio.h"
#include <string>
using namespace std;

string PuntDeInteresBotigaSolucio::getName() {
	return PuntDeInteresBase::getName();
}

unsigned int PuntDeInteresBotigaSolucio::getColor() {
	if (m_shop == "supermarket") return 0xDFFF00;
	if (m_shop == "tobacco") return 0xFF7F50;
	if (m_shop == "bakery") {
		if (m_opHour.find("06:00-22:00") != string::npos && m_movReduida)
			return 0x4CBB17;
		return 0xFA8072;
	}
	return 0xFFEA00;
}