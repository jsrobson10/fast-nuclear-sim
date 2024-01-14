
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

	virtual void interact(rod* o);
	virtual void update(double secs) = 0;
	virtual void add(val_t type, double v);
	virtual double extract(val_t type, double k, double o);
	virtual double get(val_t type) const;

	friend std::ostream& operator<<(std::ostream& o, const rod& r)
	{
		o << "Name: " << r.get_name() << "\n";
		r.display(o);
		o << "Heat: " << r.get(val_t::HEAT) << "\n";
		o << "Fast: " << r.get(val_t::N_FAST) << "\n";
		o << "Slow: " << r.get(val_t::N_SLOW) << "\n";

		return o;
	}

protected:

	double vals_in[VAL_N] = {0};
	double vals[VAL_N] = {0};

	virtual void display(std::ostream& o) const;
	virtual double get_k(val_t type) const = 0;
	virtual const char* get_name() const = 0;

	void update_rod();
};

}

