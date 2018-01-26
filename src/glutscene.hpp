#pragma once

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <utility>
#include <functional>

class glutscene {

private:
	std::pair<float, float> _canvas_size;
	std::pair<int, int> _raster_size;
	std::pair<float, float> _mouse_pos;

public:
	glutscene(std::pair<float, float> c, std::pair<int, int> r);
	void reshape(int w, int h);
	void display(void);
	void keyboard(unsigned char key, int x, int y);
	void mouse(int button, int state, int x, int y);
	void motion(int x, int y);
	void menu(int value);
};