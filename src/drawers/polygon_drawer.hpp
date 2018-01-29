#pragma once

#include <vector>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"

enum class polygon_drawer_states {
	no_points,
	one_point,
	has_points,
	final
};

class polygon_drawer : public gl_drawer {
private: 
	
	struct point {
		float x;
		float y;
		std::tuple<float, float, float> color;
	};

	fsm::finite_state_system<polygon_drawer_states, triggers> _fsm;
	// points
	std::vector<point> _points;


public:
	polygon_drawer();
	void draw(float x, float y);
	void send_trigger(trigger_data trig);
	void clear();
	bool is_complete();
};