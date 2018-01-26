
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/freeglut.h>
#endif

#include <iostream>
#include <utility>
#include <vector>
// #include "drawable.h"
#include "fsm/fsm.hpp"
#include "drawers/drawers.hpp"
#include "glutscene.hpp"

glutscene* scene;

// Doing 'pseudobinding' here so I don't have to use the boost library. 
void reshape(int w, int h) { scene->reshape(w, h); }
void display() { scene->display(); }
void keyboard(unsigned char key, int x, int y){ scene->keyboard(key, x, y); }
void mouse(int button, int state, int x, int y){ scene->mouse(button, state, x, y); }
void motion(int x, int y){ scene->motion(x, y); }
void menu(int value){ scene->menu(value); }
void idle(){ scene->idle(); }

// NOTE(clark): This code isn't great. There's finite state machines everywhere, virtual calls, and 
//              whatever I thought of to try out some new design patterns. I had a blast making it!
int main(int argc, char *argv[]) {
    
    auto canvas_size = std::make_pair(10.0f, 10.0f);
    auto raster_size = std::make_pair(800, 600);

    scene = new glutscene(canvas_size, raster_size);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
    glutInitWindowSize(raster_size.first, raster_size.second);
    glutCreateWindow("Cool Draw Application B)");

    // Callbacks
    glutReshapeFunc(reshape);
    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(motion);
    glutIdleFunc(idle);

    scene->createMenu(menu);

    glutMainLoop();

    return 0;
}