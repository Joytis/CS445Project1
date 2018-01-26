#include "glutscene.hpp"

using namespace std::placeholders;

glutscene::glutscene(std::pair<float, float> c, std::pair<int, int> r) :
	_canvas_size(c), _raster_size(r)
{

}

void glutscene::reshape(int w, int h) {
	_raster_size.first = w;
    _raster_size.second = h;
    
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluOrtho2D(0.0, _canvas_size.first, 0.0, _canvas_size.second);
    glViewport(0, 0, _raster_size.first, _raster_size.second);
    
    glutPostRedisplay();
}

void glutscene::display() {
	glClearColor(1.0, 1.0, 1.0, 0.0);
    glClear(GL_COLOR_BUFFER_BIT);
    
    // glColor3f(color);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    // Draw cursor
	glColor3f(1.0f, 0.0f, 1.0f);
    glPointSize(10.0f);
    glBegin(GL_POINTS);
    glVertex2f(_mouse_pos.first, _mouse_pos.second);
    glEnd();
    glPointSize(1.0f);

    glutSwapBuffers();
}

void glutscene::keyboard(unsigned char key, int x, int y) {
	switch(key){
        case 27:
            exit(0);
            break;
    }
}

void glutscene::mouse(int button, int state, int x, int y) {
	if(button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
        
        _mouse_pos.first = (float)x / _raster_size.first * _canvas_size.first;
        _mouse_pos.second = (float)(_raster_size.second - y) / _raster_size.second * _canvas_size.second;
        
        glutPostRedisplay();
    }
}

void glutscene::motion(int x, int y) {
	
	// mouse events are handled by OS, eventually. When using mouse in the raster window,
	//  it assumes top-left is the origin.
    // Note: the raster window created by GLUT assumes bottom-left is the origin.
	_mouse_pos.first = (float)x / _raster_size.first * _canvas_size.first;
    _mouse_pos.second = (float)(_raster_size.second - y) / _raster_size.second * _canvas_size.second;

    glutPostRedisplay();
}

void glutscene::menu(int value) {
	
}
