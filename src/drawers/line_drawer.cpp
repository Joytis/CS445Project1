#include "line_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

line_drawer::line_drawer(float width) 
	: _fsm(line_drawer_states::no_points),
	  _line_width(width)
{

	// Transitions for FSM
	_fsm.add_transition(line_drawer_states::no_points, line_drawer_states::has_points, triggers::lmouse_up);
	_fsm.add_transition(line_drawer_states::has_points, line_drawer_states::has_points, triggers::lmouse_up);
	_fsm.add_transition(line_drawer_states::has_points, line_drawer_states::final, triggers::finalize_build);

}

void line_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case line_drawer_states::no_points: {
			// do nothing
		} break;

		// Has points. 
		case line_drawer_states::has_points: {
    		glLineWidth(_line_width);
			glBegin(GL_LINES);

			auto p1 = _points.begin();
			auto p2 = std::next(p1);
			while(p2 != _points.end()) {
				// point& p = _points[i];
				glColor3f(p1->color.r, p1->color.g, p1->color.b);
				glVertex2f(p1->x, p1->y);
				glColor3f(p2->color.r, p2->color.g, p2->color.b);
				glVertex2f(p2->x, p2->y);
				p1 = p2;
				p2 = std::next(p2);
			}
			glColor3f(p1->color.r, p1->color.g, p1->color.b);
			glVertex2f(p1->x, p1->y);
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw without cursor
		case line_drawer_states::final: {
			glLineWidth(_line_width);
			glBegin(GL_LINES);

			auto p1 = _points.begin();
			auto p2 = std::next(p1);
			while(p2 != _points.end()) {
				// point& p = _points[i];
				glColor3f(p1->color.r, p1->color.g, p1->color.b);
				glVertex2f(p1->x, p1->y);
				glColor3f(p2->color.r, p2->color.g, p2->color.b);
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
	return _fsm.get_current_state() == line_drawer_states::final; 
}

