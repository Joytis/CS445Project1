#pragma once

#include <vector>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"

enum class line_drawer_states {
	no_points,
	has_points,
	final
};

class line_drawer : public gl_drawer {
private: 
	
	struct point {
		float x;
		float y;
		s_color color;
	};

	fsm::finite_state_system<line_drawer_states, triggers> _fsm;
	// points
	std::vector<point> _points;
	float _line_width;


public:
	line_drawer(float width);
	void draw(float x, float y);
	void send_trigger(trigger_data trig);
	void clear();
	bool is_complete();
};