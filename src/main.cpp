
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
#include "glutscene.hpp"

glutscene* scene;

// Doing 'pseudobinding' here so I don't have to use the boost library. 
void reshape(int w, int h) { scene->reshape(w, h); }
void display() { scene->display(); }
void keyboard(unsigned char key, int x, int y){ scene->keyboard(key, x, y); }
void mouse(int button, int state, int x, int y){ scene->mouse(button, state, x, y); }
void motion(int x, int y){ scene->motion(x, y); }
void menu(int value){ scene->menu(value); }

int main(int argc, char *argv[]) {
    // fsm::state state_1(
    //     []() { std::cout << "Enter state" << std::endl; },
    //     []() { std::cout << "Update state" << std::endl; },
    //     []() { std::cout << "Exit state" << std::endl; }
    // );


    // fsm::state state_2(
    //     []() { std::cout << "Enter state 2" << std::endl; },
    //     []() { std::cout << "Update state 2" << std::endl; },
    //     []() { std::cout << "Exit state 2" << std::endl; }
    // );

    // fsm::finite_state_system<cool, cool2> fsm(cool::cool2);
    // fsm.add_state(cool::cool2, std::move(state_1));
    // fsm.add_state(cool::cool3, std::move(state_2));
    // fsm.add_transition(cool::cool2, cool::cool3, cool2::cool);

    // fsm.update();

    // fsm.set_trigger(cool2::cool);

    // fsm.update();
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

    glutMainLoop();

    return 0;
}