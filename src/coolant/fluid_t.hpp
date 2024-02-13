
#pragma once

#include <json/json.h>

#include "vapor_pressure.hpp"

namespace sim::coolant
{

struct fluid_t
{
	const double gPl; // g/L
	const double gPmol; // g/mol
	const double jPg; // J/g		latent heat of vaporisation
	
	const coolant::vapor_pressure vapor_pressure;
	
	constexpr fluid_t(double gPl, double gPmol, double jPg, coolant::vapor_pressure vapor_pressure) :
			gPl(gPl), gPmol(gPmol), jPg(jPg),
			vapor_pressure(vapor_pressure)
	{

	}

	fluid_t(const Json::Value& node) :
			gPl(node["gPl"].asDouble()),
			gPmol(node["gPmol"].asDouble()),
			jPg(node["jPg"].asDouble()),
			vapor_pressure(node["vapor_pressure"])
	{

	}

	operator Json::Value() const
	{
		Json::Value node;

		node["gPl"] = gPl;
		node["gPmol"] = gPmol;
		node["jPg"] = jPg;

		node["vapor_pressure"] = vapor_pressure;

		return node;
	}

	constexpr double g_to_mol(double g) const { return g / gPmol; }
	constexpr double mol_to_g(double mol) const { return mol * gPmol; }
	constexpr double g_to_l(double g) const { return g / gPl; }
	constexpr double l_to_g(double l) const { return l * gPl; }
	constexpr double mol_to_l(double mol) const { return g_to_l(mol_to_g(mol)); }
	constexpr double l_to_mol(double l) const { return g_to_mol(l_to_g(l)); }
};

constexpr const fluid_t WATER = fluid_t(1000, 18, 2257, {8.07131 + 2.124903, 1730.63, 233.426 - 273.15});

}

