#include "point_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

point_drawer::point_drawer(s_color color, float size)
	: _color(color), _point_size(size), _fsm(point_drawer_states::draw_point)
{
	_fsm.add_transition(point_drawer_states::draw_point, point_drawer_states::no_point, triggers::lmouse_down);
	_fsm.add_transition(point_drawer_states::no_point, point_drawer_states::draw_point, triggers::lmouse_up);
}

void point_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case point_drawer_states::draw_point: {
			glColor3f(_color.r, _color.g, _color.b);
			glPointSize(_point_size);
			glBegin(GL_POINTS);
			glVertex2f(x, y);
			glEnd();
		} break;
		case point_drawer_states::no_point: {
			// DO nothing with no point. 
		} break;
	}


}

void point_drawer::send_trigger(trigger_data trig) {
	_fsm.set_trigger(trig.type);
}

void point_drawer::clear() {} // do nothing. 
bool point_drawer::is_complete() { return true; }