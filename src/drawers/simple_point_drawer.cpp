#include "simple_point_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using states = simple_point_drawer_states;
using std::get;

simple_point_drawer::simple_point_drawer(float point_size) 
	: _fsm(states::no_points),
	  _point_size(point_size)
{
	_fsm.add_transition(states::no_points, states::final, triggers::lmouse_up);
}

void simple_point_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case states::no_points: {} break;
		case states::final: {
			glColor3f(get<0>(_point.color), get<1>(_point.color), get<2>(_point.color));
			glPointSize(_point_size);
			glBegin(GL_POINTS);
			glVertex2f(_point.x, _point.y);
			glEnd();
		} break;
	}
}

void simple_point_drawer::send_trigger(trigger_data trig) {
	// Add a point if we're not done. 
	if(trig.type == triggers::lmouse_up && (!is_complete())) {
		_point.x = trig.lmouse_up_data.x;
		_point.y = trig.lmouse_up_data.y;
		_point.color = trig.lmouse_up_data.color;
	}

	_fsm.set_trigger(trig.type);
}

void simple_point_drawer::clear() {
	_fsm.reset();
}

bool simple_point_drawer::is_complete() {
	return _fsm.get_current_state() == states::final;
}