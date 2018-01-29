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
#include "color.hpp"

class glutscene {

private:

	s_color red;
	s_color orange;
	s_color yellow;
	s_color green;
	s_color blue;
	s_color purple;

	std::pair<float, float> _canvas_size;
	std::pair<int, int> _raster_size;
	std::pair<float, float> _mouse_pos;
	s_color _current_color;

	point_drawer _mouse_drawer;

	// set of triangles!
	std::vector<simple_point_drawer> _points;
	std::vector<triangle_drawer> _triangles;
	std::vector<quad_drawer> _quads;
	std::vector<line_drawer> _lines;
	std::vector<polygon_drawer> _polygons;

	gl_drawer* _current_drawer;

	enum class draw_state {
		triangle,
		lines,
		quad,
		polygon,
		point
	};
	draw_state _current_state;

	float _current_width;
	float _line_width;

public:
	glutscene(std::pair<float, float> c, std::pair<int, int> r);

	void cull_point_if_needed();

	void change_drawing_state(draw_state newstate);
	void createMenu(void (*menu)(int));
	void create_new_primative();
	void update_cursor();

	void reshape(int w, int h);
	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void menu(int value);
	void idle();
};