
#pragma once

#include "vapor_pressure.hpp"

namespace sim::coolant
{

struct fluid_t
{
	const double gPl; // g/L
	const double gPmol; // g/mol
	const double jPg; // J/g		latent heat of vaporisation
	const double bubble_speed; // m/s
	
	const coolant::vapor_pressure vapor_pressure;
	
	constexpr fluid_t(double gPl, double gPmol, double jPg, double bubble_speed, coolant::vapor_pressure vapor_pressure) :
			gPl(gPl), gPmol(gPmol), jPg(jPg),
			vapor_pressure(vapor_pressure),
	   		bubble_speed(bubble_speed)
	{

	}	

	constexpr double g_to_mol(double g) const { return g / gPmol; }
	constexpr double mol_to_g(double mol) const { return mol * gPmol; }
	constexpr double g_to_l(double g) const { return g / gPl; }
	constexpr double l_to_g(double l) const { return l * gPl; }
	constexpr double mol_to_l(double mol) const { return g_to_l(mol_to_g(mol)); }
	constexpr double l_to_mol(double l) const { return g_to_mol(l_to_g(l)); }
};

constexpr const fluid_t WATER = fluid_t(1000, 18, 2257, 4.1816, {8.07131, 1730.63, 233.426, 108.266, 8.14019, 1810.94, 244.485});

}

