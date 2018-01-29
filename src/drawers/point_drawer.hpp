#pragma once

#include <utility>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"
#include "color.hpp"

enum class point_drawer_states {
	draw_point,
	no_point
};

class point_drawer : public gl_drawer {
private:
	// Drawer info. 
	s_color _color;
	float _point_size;


	fsm::finite_state_system<point_drawer_states, triggers> _fsm; // state machine

public:
	point_drawer(s_color color, float size);

	void draw(float x, float y); // from gl_drawer
	void send_trigger(trigger_data trig); // from gl_drawer
	void clear(); // from gl_drawer
	bool is_complete();
};