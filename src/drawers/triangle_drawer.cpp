#include "triangle_drawer.hpp"

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>

using tds = triangle_drawer_states;
using std::get;

triangle_drawer::triangle_drawer() 
	: _fsm(tds::no_points)
{

	// Initialize the state system. 
	nopoint = fsm::state(
		[](float x, float y) {}, // enter
		[](float x, float y) {},
		[](float x, float y) {} // exit
	);

	// First point state. Draw a line
	firstpoint = fsm::state(
		[this](float x, float y) { // enter
			point& p = _points[0];
			std::cout << p.x << " " << p.y << std::endl;
			std::cout << _points.size() << std::endl;
		},
		[this](float x, float y) {
			point& p = _points[0];
			glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
			glBegin(GL_LINE_STRIP);
			glVertex2f(p.x, p.y);
			glVertex2f(x, y);
			glEnd();
		},
		[](float x, float y) {} // exit
	);

	// Second point state. Draw a couple lines. 
	secondpoint = fsm::state(
		[](float x, float y) {}, // enter
		[this](float x, float y) {
			std::cout << "2";
			// draw a point we're looking at
			glBegin(GL_LINE_STRIP);
			for(point& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glVertex2f(x, y);
			glEnd();
		},
		[](float x, float y) {} // exit
	);

	final = fsm::state(
		[](float x, float y) {}, // enter
		[this](float x, float y) {
			std::cout << "3";
			// draw a point we're looking at
			glBegin(GL_TRIANGLES);
			for(point& p : _points) {
				glColor3f(get<0>(p.color), get<1>(p.color), get<2>(p.color));
				glVertex2f(p.x, p.y);
			}
			glEnd();
		},
		[](float x, float y) {} // exit
	);

	// States
	_fsm.add_state(tds::no_points, std::move(nopoint));
	_fsm.add_state(tds::first_point, std::move(firstpoint));
	_fsm.add_state(tds::second_point, std::move(secondpoint));
	_fsm.add_state(tds::final, std::move(final));

	// Transitions
	_fsm.add_transition(tds::no_points, tds::first_point, triggers::lmouse_up);
	_fsm.add_transition(tds::first_point, tds::second_point, triggers::lmouse_up);
	_fsm.add_transition(tds::second_point, tds::final, triggers::lmouse_up);

}

void triangle_drawer::draw(float x, float y) {
	_fsm.update(x, y);
}

void triangle_drawer::send_trigger(trigger_data trig) {
	// Add a point if we're not done. 
	if(trig.type == triggers::lmouse_up && (!is_complete())) {
		// Add a point
		point p;
		p.x = trig.lmouse_up_data.x;
		p.y = trig.lmouse_up_data.y;
		p.color = trig.lmouse_up_data.color;

		std::cout << p.x  << "-" << p.y << std::endl;
		// _points.push_back(p);
		std::cout << _points.size() << std::endl;
	}

	_fsm.set_trigger(trig.type);
}

void triangle_drawer::clear() {
	_points.clear();
	_fsm.reset(0.0f, 0.0f); // screw the mouse. 
}

// We aren't complete unless we're in our final state. 
bool triangle_drawer::is_complete() { 
	return _fsm.get_current_state() == tds::final; 
}