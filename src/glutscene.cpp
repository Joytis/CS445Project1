#include "glutscene.hpp"

using namespace std::placeholders;

glutscene::glutscene(std::pair<float, float> c, std::pair<int, int> r) :
	_canvas_size(c), _raster_size(r), 
	_mouse_drawer(std::make_tuple(1.0f, 0.0f, 1.0f), 5.0f) // purple
{
    triangle_drawer tri;
    _triangles.push_back(std::move(tri));
    _current_drawer = &(_triangles.back()); // look at the back element of this vector. 
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

    _mouse_drawer.draw(_mouse_pos.first, _mouse_pos.second);

    // Draw all the stored primatives. 
    for(auto& tri : _triangles) {
        tri.draw(_mouse_pos.first, _mouse_pos.second);
    }

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
        
        trigger_data dat;
        dat.type = triggers::lmouse_down;
        dat.lmouse_down_data.x = _mouse_pos.first;
        dat.lmouse_down_data.y = _mouse_pos.second;
        dat.lmouse_down_data.color = std::make_tuple(1.0f, 0.0f, 1.0f);
        _mouse_drawer.send_trigger(dat);
        _current_drawer->send_trigger(dat);

        // glutPostRedisplay();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        trigger_data dat;
        dat.type = triggers::lmouse_up;
        dat.lmouse_up_data.x = _mouse_pos.first;
        dat.lmouse_up_data.y = _mouse_pos.second;
        dat.lmouse_up_data.color = std::make_tuple(1.0f, 0.0f, 1.0f);
        _mouse_drawer.send_trigger(dat);
        _current_drawer->send_trigger(dat);
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

void glutscene::idle() {
	glutPostRedisplay();
}
