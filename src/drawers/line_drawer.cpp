#include "line_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

using states = line_drawer_states;
using std::get;

line_drawer::line_drawer(float width) 
	: _fsm(states::no_points),
	  _line_width(width)
{

	// Transitions for FSM
	_fsm.add_transition(states::no_points, states::has_points, triggers::lmouse_up);
	_fsm.add_transition(states::has_points, states::has_points, triggers::lmouse_up);
	_fsm.add_transition(states::has_points, states::final, triggers::finalize_build);

}

void line_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case states::no_points: {
			// do nothing
		} break;

		// Has points. 
		case states::has_points: {
    		glLineWidth(_line_width);
			glBegin(GL_LINES);

			auto p1 = _points.begin();
			auto p2 = std::next(p1);
			while(p2 != _points.end()) {
				// point& p = _points[i];
				glColor3f(get<0>(p1->color), get<1>(p1->color), get<2>(p1->color));
				glVertex2f(p1->x, p1->y);
				glColor3f(get<0>(p2->color), get<1>(p2->color), get<2>(p2->color));
				glVertex2f(p2->x, p2->y);
				p1 = p2;
				p2 = std::next(p2);
			}
			glColor3f(get<0>(p1->color), get<1>(p1->color), get<2>(p1->color));
			glVertex2f(p1->x, p1->y);
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw without cursor
		case states::final: {
			glLineWidth(_line_width);
			glBegin(GL_LINES);

			auto p1 = _points.begin();
			auto p2 = std::next(p1);
			while(p2 != _points.end()) {
				// point& p = _points[i];
				glColor3f(get<0>(p1->color), get<1>(p1->color), get<2>(p1->color));
				glVertex2f(p1->x, p1->y);
				glColor3f(get<0>(p2->color), get<1>(p2->color), get<2>(p2->color));
				glVertex2f(p2->x, p2->y);
				p1 = p2;
				p2 = std::next(p2);
			}
			glEnd();
		} break;
	}

}

void line_drawer::send_trigger(trigger_data trig) {
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

void line_drawer::clear() {
	_points.clear();
	_fsm.reset(); // screw the mouse. 
}

bool line_drawer::is_complete() {
	return _fsm.get_current_state() == states::final; 
}

