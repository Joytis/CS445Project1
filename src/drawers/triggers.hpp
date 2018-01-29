#pragma once

#include "../fsm/fsm.hpp"
#include "color.hpp"

enum class triggers {
	lmouse_down = (1 << 0),
	lmouse_up = (1 << 1),
	cancel_build = (1 << 2),
	finalize_build = (1 << 3),
};
ENABLE_BITMASK_OPERATORS(triggers)

// enum and union. 
struct trigger_data {
	triggers type;
	union {
		struct { // Mouse position info
			float x;
			float y;
			s_color color;
		} lmouse_down_data; 

		struct { // Mouse position info
			float x;
			float y;
			s_color color;
		} lmouse_up_data; 

		struct {
			// Nothing for now. 
		} cancel_build_data;

		struct {
			// Nothing for now. 
		} finalize_build_data;
	};	

	trigger_data() {}
};

