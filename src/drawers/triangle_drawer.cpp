#include "triangle_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

using states = triangle_drawer_states;
using std::get;

triangle_drawer::triangle_drawer() 
	: _fsm(states::no_points)
{
	// Transitions for state system. 
	_fsm.add_transition(states::no_points, states::first_point, triggers::lmouse_up);
	_fsm.add_transition(states::first_point, states::second_point, triggers::lmouse_up);
	_fsm.add_transition(states::second_point, states::final, triggers::lmouse_up);

}

void triangle_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case states::no_points: {
			// Draw nothing. 
		} break;

		// Draw the first point and line to cursor. 
		case states::first_point: {
			point& p = _points[0];
			glBegin(GL_LINE_STRIP);
			glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
			glVertex2f(p.x, p.y);
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw two points and line to cursor. 
		case states::second_point: {
			glBegin(GL_LINE_STRIP);
			for(point& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glVertex2f(x, y);
			glEnd();
		} break;

		// Just draw a triangle!
		case states::final: {
			// draw a point we're looking at
			glBegin(GL_TRIANGLES);
			for(point& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glEnd();
		} break;
	}
}

void triangle_drawer::send_trigger(trigger_data trig) {
	// Add a point if we're not done. 
	if(trig.type == triggers::lmouse_up && (!is_complete())) {
		// Add a point
		point p;
		p.x = trig.lmouse_up_data.x;
		p.y = trig.lmouse_up_data.y;
		p.color = trig.lmouse_up_data.color;

		_points.push_back(p);
	}

	_fsm.set_trigger(trig.type);
}

void triangle_drawer::clear() {
	_points.clear();
	_fsm.reset(); // screw the mouse. 
}

// We aren't complete unless we're in our final state. 
bool triangle_drawer::is_complete() { 
	return _fsm.get_current_state() == states::final; 
}