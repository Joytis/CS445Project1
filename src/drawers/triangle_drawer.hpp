#pragma once

#include <vector>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"

enum class triangle_drawer_states {
	no_points = (1 << 0),
	first_point = (1 << 1),
	second_point = (1 << 2),
	final = (1 << 3)
};
ENABLE_BITMASK_OPERATORS(triangle_drawer_states)

class triangle_drawer : public gl_drawer {
private: 
	
	struct point {
		float x;
		float y;
		std::tuple<float, float, float> color;
	};

	fsm::finite_state_system<triangle_drawer_states, triggers> _fsm;
	// points
	std::vector<point> _points;

	// states!
	fsm::state nopoint;
	fsm::state firstpoint;
	fsm::state secondpoint;
	fsm::state final;


public:
	triangle_drawer();

	void draw(float x, float y);
	void send_trigger(trigger_data trig);
	void clear();
	bool is_complete();
};