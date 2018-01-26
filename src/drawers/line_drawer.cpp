#include "line_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

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
    		glPointSize(_line_width);
			glBegin(GL_LINE_STRIP);
			for(auto& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw without cursor
		case states::final: {
			glPointSize(_line_width);
			glBegin(GL_LINE_STRIP);
			for(auto& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
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

