
#pragma once

namespace sim::conversions::pressure
{

constexpr double torr_to_pa(double t) { return t * 133.322; }
constexpr double pa_to_torr(double p) { return p / 133.322; }

};

