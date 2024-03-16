
#include "evaporator.hpp"

#include <glm/vec3.hpp>

namespace Sim::Coolant
{

class Pool : public Evaporator
{
public:

	const glm::vec3 dimensions;

	Pool(Fluid fluid, const glm::vec3& dimensions, double temperature, double mass, double level);
	Pool(const Json::Value& node);

	operator Json::Value() const;
	double get_level_height() const;
};

};

