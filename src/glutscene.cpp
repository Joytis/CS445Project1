#include "glutscene.hpp"

using namespace std::placeholders;

glutscene::glutscene(std::pair<float, float> c, std::pair<int, int> r) :
	_canvas_size(c), _raster_size(r), 
    _current_color(red),
    _current_state(draw_state::triangle),
    _current_width(5.0),
    _mouse_drawer(_current_color, _current_width)
{
    triangle_drawer tri;
    _triangles.push_back(std::move(tri));
    _current_drawer = &(_triangles.back()); // look at the back element of this vector. 
    update_cursor();
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
    for(auto& tri : _triangles) 
        tri.draw(_mouse_pos.first, _mouse_pos.second);
    for(auto& quad : _quads) 
        quad.draw(_mouse_pos.first, _mouse_pos.second);
    for(auto& line : _lines) 
        line.draw(_mouse_pos.first, _mouse_pos.second);
    for(auto& point : _points) 
        point.draw(_mouse_pos.first, _mouse_pos.second);
    for(auto& polygon : _polygons) 
        polygon.draw(_mouse_pos.first, _mouse_pos.second);

        
    if(_current_drawer->is_complete()) {
        create_new_primative();
    }

    glutSwapBuffers();
}

void glutscene::keyboard(unsigned char key, int x, int y) {
	switch(key){
        case 27: { // escape
            exit(0);
        } break;
        case 32: { // space
            trigger_data dat;
            dat.type = triggers::finalize_build;
            _mouse_drawer.send_trigger(dat);
            _current_drawer->send_trigger(dat);
        } break;
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
        dat.lmouse_down_data.color = _current_color;
        _mouse_drawer.send_trigger(dat);
        _current_drawer->send_trigger(dat);

        // glutPostRedisplay();
    }

    if(button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
        trigger_data dat;
        dat.type = triggers::lmouse_up;
        dat.lmouse_up_data.x = _mouse_pos.first;
        dat.lmouse_up_data.y = _mouse_pos.second;
        dat.lmouse_up_data.color = _current_color;
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

// Forward the callback to openGL
void glutscene::createMenu(void (*menu)(int)) {

    int colorMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Red", 00);
    glutAddMenuEntry("Orange", 01);
    glutAddMenuEntry("Yellow", 02);
    glutAddMenuEntry("Green", 03);
    glutAddMenuEntry("Blue", 04);
    glutAddMenuEntry("Purple", 05);

    int lineWidthMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Small", 30);
    glutAddMenuEntry("Medium", 31);
    glutAddMenuEntry("Largs", 32);

    int shapeMenu = glutCreateMenu(menu);
    glutAddMenuEntry("Point", 15);
    glutAddMenuEntry("Lines", 11);
    glutAddMenuEntry("Triangle", 10);
    glutAddMenuEntry("Quad", 12);
    glutAddMenuEntry("Polygon", 14);


    glutCreateMenu(menu);
    glutAddSubMenu("Shapes", shapeMenu);
    glutAddSubMenu("Colors", colorMenu);
    glutAddSubMenu("PointSize", lineWidthMenu);
    glutAddMenuEntry("Clear", 20);
    glutAddMenuEntry("Exit", 21);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void glutscene::update_cursor() {
    // just make a new one. 
    _mouse_drawer = point_drawer(_current_color, _current_width);
}

void glutscene::menu(int value) {
    switch(value) {
        // Change current rendering color. 
        case 00 : { 
            _current_color = glutscene::red; 
            update_cursor();
        } break;
        case 01 : { 
            _current_color = glutscene::orange; 
            update_cursor();
        } break;
        case 02 : { 
            _current_color = glutscene::yellow; 
            update_cursor();
        } break;
        case 03 : { 
            _current_color = glutscene::green; 
            update_cursor();
        } break;
        case 04 : { 
            _current_color = glutscene::blue; 
            update_cursor();
        } break;
        case 05 : { 
            _current_color = glutscene::purple; 
            update_cursor();
        } break;

        // Update drawing state
        case 10 : { 
            change_drawing_state(draw_state::triangle);
            update_cursor();
        } break;
        case 11 : { 
            change_drawing_state(draw_state::lines);
            update_cursor();
        } break;
        case 12 : { 
            change_drawing_state(draw_state::quad);
            update_cursor();
        } break;
        case 14 : { 
            change_drawing_state(draw_state::polygon);
            update_cursor();
        } break;
        case 15 : { 
            change_drawing_state(draw_state::point);
            update_cursor();
        } break;

        // Clear screen
        case 20 : {
            _triangles.clear();
            _quads.clear();
            _lines.clear();
            _points.clear();
            _polygons.clear();

            create_new_primative();
        } break;

        case 21 : { exit(0); } break;

        // Line cursor ssizes. 
        case 30 : {
            _current_width = 5.0f;
            update_cursor();
        } break;
        case 31 : {
            _current_width = 10.0f;
            update_cursor();
        } break;
        case 32 : {
            _current_width = 15.0f;
            update_cursor();
        } break;
    }
}

void glutscene::create_new_primative() {
    // Make a new shape to draw. 
    switch(_current_state) {
        // points
        case draw_state::point: {
            simple_point_drawer point(_current_width);
            _points.push_back(std::move(point));
            _current_drawer = &(_points.back());
        } break;

        // triangles 
        case draw_state::triangle: {
            triangle_drawer tri;
            _triangles.push_back(std::move(tri));
            _current_drawer = &(_triangles.back());
        } break;

        // lines
        case draw_state::lines: {
            line_drawer line(_current_width);
            _lines.push_back(std::move(line));
            _current_drawer = &(_lines.back());
        } break;

        // Quads
        case draw_state::quad: {
            quad_drawer quad;
            _quads.push_back(std::move(quad));
            _current_drawer = &(_quads.back());
        } break;

        // polygons
        case draw_state::polygon: {
            polygon_drawer polygon;
            _polygons.push_back(std::move(polygon));
            _current_drawer = &(_polygons.back());
        } break;
    }
}

void glutscene::change_drawing_state(draw_state newstate) {
    if(!_current_drawer->is_complete()) {
        switch(_current_state) {
            case draw_state::triangle: { _triangles.pop_back(); } break;
            case draw_state::lines: { _lines.pop_back(); } break;
            case draw_state::quad: { _quads.pop_back(); } break;
            case draw_state::polygon: { _polygons.pop_back(); } break;
            case draw_state::point: { _points.pop_back(); } break;
        }
    }
    _current_state = newstate;

    create_new_primative();
}

void glutscene::idle() {
	glutPostRedisplay();
}
