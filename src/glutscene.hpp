#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <utility>
#include <functional>
#include <vector>

#include "drawers/drawers.hpp"

class glutscene {

private:
	std::pair<float, float> _canvas_size;
	std::pair<int, int> _raster_size;
	std::pair<float, float> _mouse_pos;

	point_drawer _mouse_drawer;

	// set of triangles!
	std::vector<triangle_drawer> _triangles;

	gl_drawer* _current_drawer;

public:
	glutscene(std::pair<float, float> c, std::pair<int, int> r);
	void reshape(int w, int h);
	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void menu(int value);
	void idle();
};