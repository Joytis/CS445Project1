#pragma once

#include <vector>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"
#include "color.hpp"

enum class triangle_drawer_states {
	no_points,
	first_point,
	second_point,
	final
};

class triangle_drawer : public gl_drawer {
private: 
	
	struct point {
		float x;
		float y;
		s_color color;
	};

	fsm::finite_state_system<triangle_drawer_states, triggers> _fsm;
	// points
	std::vector<point> _points;


public:
	triangle_drawer();

	void draw(float x, float y);
	void send_trigger(trigger_data trig);
	void clear();
	bool is_complete();
};