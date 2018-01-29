#include "polygon_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

polygon_drawer::polygon_drawer() 
	: _fsm(polygon_drawer_states::no_points)
{

	// Transitions for FSM
	_fsm.add_transition(polygon_drawer_states::no_points, polygon_drawer_states::one_point, triggers::lmouse_up);
	_fsm.add_transition(polygon_drawer_states::one_point, polygon_drawer_states::has_points, triggers::lmouse_up);
	_fsm.add_transition(polygon_drawer_states::has_points, polygon_drawer_states::has_points, triggers::lmouse_up);
	_fsm.add_transition(polygon_drawer_states::has_points, polygon_drawer_states::final, triggers::finalize_build);

}

void polygon_drawer::draw(float x, float y) {
	_fsm.update();

	switch(_fsm.get_current_state()) {
		case polygon_drawer_states::no_points: { } break;

		// Draw the first point and line to cursor. 
		case polygon_drawer_states::one_point: {
			point& p = _points[0];
			glBegin(GL_LINE_STRIP);
			glColor3f(p.color.r, p.color.g, p.color.b);
			glVertex2f(p.x, p.y);
			glVertex2f(x, y);
			glEnd();
		} break;

		// Has points. 
		case polygon_drawer_states::has_points: {
			glBegin(GL_POLYGON);
			for(auto& p : _points) {
				glColor3f(p.color.r, p.color.g, p.color.b);
				glVertex2f(p.x, p.y);
			}
			glVertex2f(x, y);
			glEnd();
		} break;

		// Draw without cursor
		case polygon_drawer_states::final: {
			glBegin(GL_POLYGON);
			for(auto& p : _points) {
				glColor3f(p.color.r, p.color.g, p.color.b);
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
	return _fsm.get_current_state() == polygon_drawer_states::final; 
}

