#include <stdio.h>
#include <math.h>

#include "ImageClass.h"
#include "Temporizador.h"


ImageClass image;
Temporizador timer;
float accum_delta_t = 0;
const int window_width = 800;
const int window_height = 800;

char *image_path;
float n_seconds;
float pos_x, pos_y;
float dir_x, dir_y;

float zoom_h, zoom_v;



void init()
{
    int r = image.Load(image_path);
    if (!r) exit(1);

    // adjust image size
    zoom_h = (window_width/5.0)/(float)image.SizeX();
    zoom_v = (window_height/5.0)/(float)image.SizeY();
    image.SetZoomH(zoom_h);
    image.SetZoomV(zoom_v);

    // adjust image position
    pos_x = 0;
    pos_y = 0;
    image.SetPos(pos_x, pos_y);

    // set movement direction
    dir_x = 1.0f;
    dir_y = 0.0f;
}


void display(void)
{
    float dt = timer.getDeltaT();
    accum_delta_t += dt;
    if (accum_delta_t > 1)
    {
        accum_delta_t = 0;
        printf("FPS: %f\n", 1.0f/dt);
    }

    float stop_x = window_width - (image.SizeX()*zoom_h);
    float stop_y = window_height - (image.SizeY()*zoom_v);

    if(pos_x < stop_x && pos_y < stop_y)
    {
        float module = sqrt(pow(dir_x, 2) + pow(dir_y, 2));
        float unit_vector_x = dir_x/module;
        float unit_vector_y = dir_y/module;

        float velocity_x = stop_x/n_seconds;
        float velocity_y = stop_y/n_seconds;

        pos_x += unit_vector_x * velocity_x * dt;
        pos_y += unit_vector_y * velocity_y * dt;

        image.SetPos(pos_x, pos_y);
    }

    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    image.Display();
    glutSwapBuffers();
}


void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, w, h);
    gluOrtho2D(0, w, 0, h);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}


void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
    case 27:
        image.Delete();
        exit (0);
        break;
    default:
        break;
    }
}


int main (int argc, char** argv)
{
    if(argc != 3){
        printf("Usage: example_exercise1 <image_path> <n_seconds>\n");
        exit(1);
    }

    image_path = argv[1];
    n_seconds = atof(argv[2]);

    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH | GLUT_RGB );
    glutInitWindowPosition(0,0);
    glutInitWindowSize(window_width, window_height);
    glutCreateWindow("Exercise 1 - Animation");

    init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);

    glutIdleFunc(display);

    glutMainLoop();

    free(image_path);
    return 0;
}


