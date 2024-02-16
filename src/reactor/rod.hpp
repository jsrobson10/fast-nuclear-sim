
#pragma once

#include <json/json.h>

#include <ostream>
#include <glm/vec4.hpp>
#include <memory>

namespace sim::reactor
{

class Rod
{
public:
	
	bool selected = false;
	void* reactor = nullptr;
	static const int VAL_N = 3;
	
	enum val_t
	{
		HEAT = 0,
		N_SLOW = 1,
		N_FAST = 2,
	};

	constexpr Rod() {};
	Rod(const Json::Value& node);

	virtual ~Rod() {};
	virtual void interact(Rod* o, double secs);
	virtual void update(double secs) { }
	virtual void add(val_t type, double v);
	virtual double extract(val_t type, double s, double k, double o);
	virtual double get(val_t type) const;
	virtual std::unique_ptr<Rod> clone() const { return std::make_unique<Rod>(*this); }
	virtual double get_energy_output() const { return 0; }
	virtual int get_id() const { return 0; }
	
	virtual glm::vec4 get_colour() const { return {0, 0, 0, 0}; }
	glm::vec4 get_heat_colour() const;

	virtual bool has_sensors(val_t t) const { return false; }
	virtual bool should_display() const { return false; }
	virtual bool should_select() const { return false; }
	virtual void update_selected(double a) { }

	double get_flux() const;
	double get_side_area() const;
	double get_volume() const;
	
	constexpr void toggle_selected() { selected = !selected; }

	friend std::ostream& operator<<(std::ostream& o, const Rod& r)
	{
		if(!r.should_display()) return o;

		o << r.get_name() << "\n";

		r.display(o);

		o << "Heat: " << r.get(val_t::HEAT) << " C\n";
		o << "Fast: " << r.get(val_t::N_FAST) << " mol\n";
		o << "Slow: " << r.get(val_t::N_SLOW) << " mol\n";

		return o;
	}

	virtual Json::Value serialize() const;

protected:

	double vals[VAL_N] = {0};
	double vals_n[VAL_N] = {0};

	virtual void display(std::ostream& o) const { };
	virtual double get_k(val_t type) const { return 0; }
	virtual const char* get_name() const { return "Empty"; }

	void update_rod(double secs);
};

}

