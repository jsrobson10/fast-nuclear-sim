
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "vessel.hpp"
#include "../../reactor/rod.hpp"
#include "../../reactor/control/boron_rod.hpp"
#include "../../system.hpp"
#include "../../util/streams.hpp"
#include "../data/font.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <sstream>

using namespace Sim::Graphics::Monitor;
using namespace Sim::Graphics::Data;
using namespace Sim::Util::Streams;

Vessel::Vessel(const Model& model) : Data::MeshGen("vessel")
{
	mat = model.load_matrix("translation_monitor_2");
}

void Vessel::remesh_static(Mesh& rmesh)
{
	std::stringstream ss;

	ss << "Reactor Vessel\n\n";
	ss << "Heat\n";
	ss << "Steam\n";
	ss << "Pressure\n";
	ss << "Level\n";
	ss << "Void Ratio\n\n";
	ss << "Reactor Core\n\n";
	ss << "Neutron Flux\n\n";
	ss << "Energy Output\n\n";
	ss << "Temperature\nMin\nMax\n\n";
	ss << "Control Rods\nMin\nMax\nSpeed\n";

	rmesh.add(Data::Fonts::BASE.load_text(ss.str(), 0.04), mat, true);
}

void Vessel::remesh_slow(Mesh& rmesh)
{
	std::stringstream ss;
	Sim::System& sys = *System::active;

	double temp_min, temp_max;
	double crod_min = INFINITY, crod_max = -INFINITY;

	sys.reactor.get_stats(Sim::Reactor::Rod::val_t::HEAT, temp_min, temp_max);

	for(int i = 0; i < sys.reactor.size; i++)
	{
		Sim::Reactor::Rod* r = sys.reactor.rods[i].get();

		if(r->get_id() != 5)
		{
			continue;
		}

		auto br = (Sim::Reactor::Control::BoronRod*)r;
		double v = br->get_inserted();

		if(v > crod_max)
		{
			crod_max = v;
		}

		if(v < crod_min)
		{
			crod_min = v;
		}
	}

	ss << "\n\n";
	ss << show( sys.vessel.get_heat() ) << " C\n";
	show_units( ss, sys.vessel.get_steam() ) << "g\n";
	show_units( ss, sys.vessel.get_pressure() ) << "Pa\n";
	ss << show( sys.vessel.get_level() / 1000 ) << " / " << show( sys.vessel.get_volume() / 1000 ) << " kL\n";
	ss << show( sys.vessel.get_void_ratio() * 100 ) << " %\n\n\n\n";
	show_units( ss, sys.reactor.get_flux() ) << "n/cm2/s\n";
	show_units( ss, sys.reactor.get_energy_output() ) << "W\n\n\n";
//	ss << show( sys.reactor.flux_rate * 100 ) << " %/s\n\n\n";
	ss << show( temp_min ) << " C\n";
	ss << show( temp_max ) << " C\n\n\n";
	ss << show( 100 - crod_max * 100 ) << " %\n";
	ss << show( 100 - crod_min * 100 ) << " %\n";
	ss << show( sys.reactor.rod_speed * 100, 1e6 ) << " %/s";
	if(sys.reactor.rod_speed == 0) ss << " (Stopped)";
	ss << "\n";

	Mesh mesh = Data::Fonts::BASE.load_text(ss.str(), 0.04);
	rmesh.add(mesh, glm::translate(mat, glm::vec3(0.5, 0, 0)));
}

