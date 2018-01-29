#include "polygon_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using states = polygon_drawer_states;
using std::get;

polygon_drawer::polygon_drawer() 
	: _fsm(states::no_points)
{

	// Transitions for FSM
	_fsm.add_transition(states::no_points, states::one_point, triggers::lmouse_up);
	_fsm.add_transition(states::one_point, states::has_points, triggers::lmouse_up);
	_fsm.add_transition(states::has_points, states::has_points, triggers::lmouse_up);
	_fsm.add_transition(states::has_points, states::final, triggers::finalize_build);

}

void polygon_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case states::no_points: { } break;

		// Draw the first point and line to cursor. 
		case states::one_point: {
			point& p = _points[0];
			glBegin(GL_LINE_STRIP);
			glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
			glVertex2f(p.x, p.y);
			glVertex2f(x, y);
			glEnd();
		} break;

		// Has points. 
		case states::has_points: {
			glBegin(GL_POLYGON);
			for(auto& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw without cursor
		case states::final: {
			glBegin(GL_POLYGON);
			for(auto& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glEnd();
		} break;
	}

}

void polygon_drawer::send_trigger(trigger_data trig) {
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

void polygon_drawer::clear() {
	_points.clear();
	_fsm.reset(); // screw the mouse. 
}

bool polygon_drawer::is_complete() {
	return _fsm.get_current_state() == states::final; 
}

