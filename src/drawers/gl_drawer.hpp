#pragma once

#include "triggers.hpp"

class gl_drawer {
public:
	virtual void draw(float x, float y) = 0;
	virtual void send_trigger(trigger_data trig) = 0;
	virtual bool is_complete() = 0;
	virtual void clear() = 0;
};