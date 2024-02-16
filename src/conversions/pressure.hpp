
#pragma once

namespace Sim::Conversions::Pressure
{

constexpr double torr_to_pa(double t) { return t * 133.322; }
constexpr double pa_to_torr(double p) { return p / 133.322; }
constexpr double mmhg_to_pa(double t) { return t * 133.3224; }
constexpr double pa_to_mmhg(double p) { return p / 133.3224; }

};

