#pragma once

#include <vector>

#include "triggers.hpp"
#include "gl_drawer.hpp"
#include "../fsm/fsm.hpp"
#include "color.hpp"

enum class simple_point_drawer_states {
	no_points,
	final
};

class simple_point_drawer : public gl_drawer {
private:

	struct point {
		float x;
		float y;
		s_color color;
	};

	fsm::finite_state_system<simple_point_drawer_states, triggers> _fsm;
	point _point;
	float _point_size;

public:
	simple_point_drawer(float point_size);

	void draw(float x, float y);
	void send_trigger(trigger_data trig);
	void clear();
	bool is_complete();
};