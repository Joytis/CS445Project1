#pragma once

#include "../fsm/fsm.hpp"

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
			std::tuple<float, float, float> color;
		} lmouse_down_data; 

		struct { // Mouse position info
			float x;
			float y;
			std::tuple<float, float, float> color;
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