#pragma once

#include <utility>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"

enum class point_drawer_states {
	draw_point = (1 << 0),
	no_point = (1 << 1),
};
ENABLE_BITMASK_OPERATORS(point_drawer_states)

class point_drawer : public gl_drawer {
private:
	// Drawer info. 
	std::tuple<float, float, float> _color;
	float _point_size;


	fsm::finite_state_system<point_drawer_states, triggers> _fsm; // state machine

public:
	point_drawer(std::tuple<float, float, float> color, float size);

	void draw(float x, float y); // from gl_drawer
	void send_trigger(trigger_data trig); // from gl_drawer
	void clear(); // from gl_drawer
	bool is_complete();
};