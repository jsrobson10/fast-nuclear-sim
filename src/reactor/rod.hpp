
#pragma once

#include <ostream>

namespace sim::reactor
{

class rod
{
public:

	static const int VAL_N = 3;
	
	enum val_t
	{
		HEAT = 0,
		N_SLOW = 1,
		N_FAST = 2
	};

	virtual void interact(rod* o, double secs);
	virtual void update(double secs) { };
	virtual void add(val_t type, double v);
	virtual double extract(val_t type, double k, double o);
	virtual double get(val_t type) const;

	virtual bool should_display() const { return false; }
	virtual bool should_select() const { return false; }
	virtual void update_selected(double a) { }
	
	constexpr void toggle_selected() { selected = !selected; }
	constexpr bool is_selected() { return selected; }

	friend std::ostream& operator<<(std::ostream& o, const rod& r)
	{
		if(!r.should_display()) return o;

		o << r.get_name() << "\n";
		r.display(o);
//		o << "Heat: " << r.get(val_t::HEAT) << "\n";
		o << "Fast: " << r.get(val_t::N_FAST) << "\n";
		o << "Slow: " << r.get(val_t::N_SLOW) << "\n";

		return o;
	}

protected:

	double vals_in[VAL_N] = {0};
	double vals[VAL_N] = {0};
	bool selected = false;

	virtual void display(std::ostream& o) const { };
	virtual double get_k(val_t type) const { return 0; }
	virtual const char* get_name() const { return "Empty"; }

	void update_rod();
};

}

