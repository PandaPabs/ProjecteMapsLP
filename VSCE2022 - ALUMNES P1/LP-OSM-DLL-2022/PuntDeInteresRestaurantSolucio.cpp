#include "pch.h"
#include "PuntDeInteresRestaurantSolucio.h"
using namespace std;

string PuntDeInteresRestaurantSolucio::getName() {
	return PuntDeInteresBase::getName();
}

unsigned int PuntDeInteresRestaurantSolucio::getColor() {
	if (m_cuisine == "pizza") return 0x7FFFD4;
	if (m_cuisine == "chinese") return 0x00FFFF;
	if (m_movReduida) return 0x5D3FD3;
	return PuntDeInteresBase::getColor();
}