
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
	virtual double extract(val_t type, double s, double k, double o);
	virtual double get(val_t type) const;

	virtual bool should_display() const { return false; }
	virtual bool should_select() const { return false; }
	void update_rod_selected(int m);
	
	constexpr void toggle_selected() { selected = !selected; }
	constexpr bool is_selected() const { return selected; }

	friend std::ostream& operator<<(std::ostream& o, const rod& r)
	{
		if(!r.should_display()) return o;

		o << r.get_name() << "\n";

		if(r.is_selected())
		{
			o << "Speed: " << r.get_speed() << "\n";
		}

		r.display(o);

		o << "Heat: " << r.get(val_t::HEAT) << " C\n";
		o << "Fast: " << r.get(val_t::N_FAST) << " mol\n";
		o << "Slow: " << r.get(val_t::N_SLOW) << " mol\n";

		return o;
	}

protected:

	double vals[VAL_N] = {0};
	bool selected = false;
	int motion = 0;

	virtual void display(std::ostream& o) const { };
	virtual double get_k(val_t type) const { return 0; }
	virtual const char* get_name() const { return "Empty"; }
	virtual void update_selected(double a) { }

	void update_rod(double secs);
	double get_speed() const;
};

}

