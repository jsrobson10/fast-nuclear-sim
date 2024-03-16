
#include "pool.hpp"

using namespace Sim::Coolant;

Pool::Pool(const Json::Value& node)
	: Evaporator(node)
	, dimensions(node["dimensions"]["x"].asDouble(), node["dimensions"]["y"].asDouble(), node["dimensions"]["z"].asDouble())
{

}

Pool::Pool(Fluid fluid, const glm::vec3& dimensions, double heat, double mass, double level)
	: Evaporator(fluid, dimensions.x * dimensions.y * dimensions.z * 1000, mass, level)
	, dimensions(dimensions)
{
	this->heat = heat;
	this->level = level;
}

Pool::operator Json::Value() const
{
	Json::Value node = Evaporator::operator Json::Value();
	node["dimensions"]["x"] = dimensions.x;
	node["dimensions"]["y"] = dimensions.y;
	node["dimensions"]["z"] = dimensions.z;
	return node;
}

double Pool::get_level_height() const
{
	double ratio = level / volume;
	return dimensions.z * ratio;
}

