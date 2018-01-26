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
	typedef std::tuple<float, float, float> color_type;

	const color_type red = { 1.0, 0.0, 0.0 };
	const color_type orange = { 1.0, 0.64, 0.0 };
	const color_type yellow = { 1.0, 0.95, 0.0 };
	const color_type green = { 0.0, 1.0, 0.0 };
	const color_type blue = { 0.0, 0.0, 1.0 };
	const color_type purple = { 0.56, 0.0, 0.8 };

	std::pair<float, float> _canvas_size;
	std::pair<int, int> _raster_size;
	std::pair<float, float> _mouse_pos;
	std::tuple<float, float, float> _current_color;

	point_drawer _mouse_drawer;

	// set of triangles!
	std::vector<triangle_drawer> _triangles;
	std::vector<quad_drawer> _quads;
	std::vector<line_drawer> _lines;

	gl_drawer* _current_drawer;

	enum class draw_state {
		triangle,
		lines,
		quad,
		circle,
		polygon
	};
	draw_state _current_state;

	float _current_width = 3.0f;

public:
	glutscene(std::pair<float, float> c, std::pair<int, int> r);

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