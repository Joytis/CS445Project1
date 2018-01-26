#include "point_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

using std::get;
using pds = point_drawer_states;

point_drawer::point_drawer(std::tuple<float, float, float> color, float size)
	: _color(color), _point_size(size), _fsm(point_drawer_states::draw_point)
{
	// Initialize the state system. 
	fsm::state draw_state(
		[](float x, float y) {}, // enter
		[this](float x, float y) { //update
			// Draw a point. 
			glColor3f(get<0>(_color), get<1>(_color), get<2>(_color));
			glPointSize(_point_size);
			glBegin(GL_POINTS);
			glVertex2f(x, y);
			glEnd();
		},
		[](float x, float y) {} // exit
	);

	fsm::state no_draw_state(
		[](float x, float y) {}, // enter
		[](float x, float y) {}, // update
		[](float x, float y) {} // exit
	);

	_fsm.add_state(pds::draw_point, std::move(draw_state));
	_fsm.add_state(pds::no_point, std::move(no_draw_state));

	_fsm.add_transition(pds::draw_point, pds::no_point, triggers::lmouse_down);
	_fsm.add_transition(pds::no_point, pds::draw_point, triggers::lmouse_up);
}

void point_drawer::draw(float x, float y) {
	_fsm.update(x, y);
}

void point_drawer::send_trigger(trigger_data trig) {
	_fsm.set_trigger(trig.type);
}

void point_drawer::clear() {} // do nothing. 
bool point_drawer::is_complete() { return true; }