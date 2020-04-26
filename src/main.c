#include <GL/glut.h>
#include "SOIL/SOIL.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include "model.h"
#include "draw.h"
#include "camera.h"
#include <sys/time.h>

#define resolution 1   // 1: 1920*1080   0: 1366*768
#define fullscreen 0   // 1: fullscreen  0: windowed
#define VIEWPORT_RATIO (16.0 / 9.0)

  GLfloat light_position[] =	{0.0, 0.0, 0.0, 0.0};
  GLfloat light_ambient[] = 	{1.0, 1.0, 1.0, 1.0};


typedef GLubyte Pixel[3];
struct Camera camera;
struct Action action;
struct Action
{
	int move_forward;
	int move_backward;
	int step_left;
	int step_right;
	int move_up;
	int move_down;
	int rotate_earth_in_galaxy;
	int move_earth_in_galaxy;
	int call_falcon;
	int increase_light;
	int decrease_light;
};

World world;
Rotate rotate;
Move move;

int WINDOW_WIDTH;
int WINDOW_HEIGHT;
int mouse_x, mouse_y;
int previous_time;
int help, help_on = 0;
float speed = 30;
float angle = 135;
double degree = 0;
double distance_a = 4000;
double distance_b = 3000;
int falcon=0;


double calc_elapsed_time()
{
	int current_time;
	double elapsed_time;

	current_time = glutGet(GLUT_ELAPSED_TIME);
	elapsed_time = (double)(current_time - previous_time) / 1000.0;
	previous_time=current_time;

	return elapsed_time;
}

void update_camera_position(struct Camera* camera, double elapsed_time)
{
	double distance;

	distance = elapsed_time * MOVE_SPEED*speed;

	if (action.move_forward == TRUE) {
		move_camera_forward(camera, distance);
	}

	if (action.move_backward == TRUE) {
		move_camera_backward(camera, distance);
	}

	if (action.step_left == TRUE) {
		step_camera_left(camera, distance);
	}

	if (action.step_right == TRUE) {
		step_camera_right(camera, distance);
	}

	if (action.move_up == TRUE) {
		move_camera_up(camera, distance);
	}

	if (action.move_down == TRUE) {
		move_camera_down(camera, distance);
	}


	don_not_head_up_against_the_wall (camera, move);


}



void specialFunc(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_F1:
		if (help_on) {
			help_on = 0;
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, light_ambient);
		 	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
		}else {
			help_on = 1;
			GLfloat ones[] = { 1, 1, 1, 1 };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, ones);
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ones);


		}
	}

}

void reshape(GLsizei width, GLsizei height)
{
	WINDOW_WIDTH = width;
	WINDOW_HEIGHT = height;

    int x, y, w, h;
    double ratio;

    ratio = (double)width / height;
    if (ratio > VIEWPORT_RATIO) {
        w = (int)((double)height * VIEWPORT_RATIO);
        h = height;
        x = (width - w) / 2;
        y = 0;
    }
    else {
        w = width;
        h = (int)((double)width / VIEWPORT_RATIO);
        x = 0;
        y = (height - h) / 2;
    }

    glViewport (x, y, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (!help_on){
	gluPerspective(50.0, VIEWPORT_RATIO, 0.1, 20000.0);
	}
	else
		gluOrtho2D(0, width, height, 0);
}



void draw_help() {

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, help);
	glBegin(GL_QUADS);
	glTexCoord2f(0, 0);
	glVertex3f(0, 0, 0);
	glTexCoord2f(1, 0);
	glVertex3f(WINDOW_WIDTH, 0, 0);
	glTexCoord2f(1, 1);
	glVertex3f(WINDOW_WIDTH, WINDOW_HEIGHT, 0);
	glTexCoord2f(0, 1);
	glVertex3f(0, WINDOW_HEIGHT, 0);
	glEnd();
	glDisable(GL_TEXTURE_2D);
	reshape(WINDOW_WIDTH, WINDOW_HEIGHT);
	glutSwapBuffers();

}



void rotation_of_objects(Rotate* rotate) {

	if (action.rotate_earth_in_galaxy == TRUE)
	rotate->earth_rotation+=0.5;

	rotate->sun_rotation+=0.1;

}

void movement_of_objects (Move* move){


if (action.move_earth_in_galaxy == TRUE){
		degree+=0.2;
		double angle = degree_to_radian(degree);
		move->earth.x = cos(angle) * distance_a;
		move->earth.y = sin(angle) * distance_b;
		move->earth.z = 0;
		}
	else
		if (move->earth.x == 0){
		move->earth.x = 4000;
		move->earth.y = 0;
		move->earth.z = 0;
		}


if (action.call_falcon == TRUE && move->falcon.x<6000){
	if (falcon==0){
		move->falcon.x=-6000;
		move->falcon.y=1500;
		move->falcon.z=400;
	}
	move->falcon.x+=1000.0*calc_elapsed_time();
	falcon=1;			}
	else
		if (action.call_falcon == TRUE && move->falcon.x>=6000){
		move->falcon.x=-20000;
		action.call_falcon = FALSE;
		falcon=0;}
	else
		if (action.call_falcon == FALSE){
		move->falcon.x=-20000;}


}


void display() {
	if (!help_on) {
		double elapsed_time;
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glMatrixMode(GL_MODELVIEW);
		elapsed_time = calc_elapsed_time();
		update_camera_position(&camera, elapsed_time);
		draw_environment(world, &rotate, move);
		glLightfv(GL_LIGHT0, GL_POSITION, light_position);
		glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);
		glMatrixMode(GL_PROJECTION);
		set_view_point(&camera);
		movement_of_objects(&move);
		rotation_of_objects(&rotate);
		glutSwapBuffers();

	}
	else {
		draw_help();
	}
}


void mouse_handler(int button, int state, int x, int y)
{
	mouse_x = x;
	mouse_y = y;
}


void motion_handler(int x, int y)
{
	double horizontal, vertical;

	horizontal = mouse_x - x;
	vertical = mouse_y - y;

	rotate_camera(&camera, horizontal, vertical);

	mouse_x = x;
	mouse_y = y;

	glutPostRedisplay();
}


void key_handler(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		action.move_forward = TRUE;
		break;
	case 's':
		action.move_backward = TRUE;
		break;
	case 'a':
		action.step_left = TRUE;
		break;
	case 'd':
		action.step_right = TRUE;
		break;
	case 'c':
		action.move_down = TRUE;
		break;
	case 32:
		action.move_up = TRUE;
		break;
	case 'q':
		if (action.rotate_earth_in_galaxy == FALSE)
			action.rotate_earth_in_galaxy = TRUE;
		else action.rotate_earth_in_galaxy = FALSE;
		break;
	case 'e':
		if (action.move_earth_in_galaxy == FALSE)
			action.move_earth_in_galaxy = TRUE;
		else action.move_earth_in_galaxy = FALSE;
		break;
	case 'f':
		if (action.call_falcon == FALSE){
			action.call_falcon = TRUE;
			}
		break;
	case '+':
		{
			if (light_ambient[0] < 1) {
				light_ambient[0] = light_ambient[1] = light_ambient[2]+= 0.1;
			}
		}
		break;
	case '-':
		{
			if (light_ambient[0] > 0) {
				light_ambient[0] = light_ambient[1] = light_ambient[2]-= 0.1;
			}
			}
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}


void key_up_handler(unsigned char key, int x, int y)
{
	switch (key) {
	case 'w':
		action.move_forward = FALSE;
		break;
	case 's':
		action.move_backward = FALSE;
		break;
	case 'a':
		action.step_left = FALSE;
		break;
	case 'd':
		action.step_right = FALSE;
		break;
	case 'c':
		action.move_down = FALSE;
		break;
	case 32:
		action.move_up = FALSE;
		break;
	case 27:
		exit(0);
	}

	glutPostRedisplay();
}


void idle()
{
	
	glutPostRedisplay();
}


GLuint load_texture(const char* filename) {
	glPixelStorei(GL_UNPACK_ALIGNMENT,1);
	GLuint texture_name;
	Pixel* image;
	glGenTextures(1, &texture_name);

	int width;
	int height;

	image = (Pixel*)SOIL_load_image(filename, &width, &height, 0, SOIL_LOAD_RGBA); // @suppress("Symbol is not resolved")

	glBindTexture(GL_TEXTURE_2D, texture_name);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, (Pixel*)image);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	SOIL_free_image_data(image);
	return texture_name;
}


void set_callbacks() {
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(key_handler);
	glutKeyboardUpFunc(key_up_handler);
	glutMouseFunc(mouse_handler);
	glutMotionFunc(motion_handler);
	glutIdleFunc(idle);
	glutSpecialFunc(specialFunc);
	if (fullscreen==1) glutFullScreen();
}


void initialize()
{
	glClearColor (0.0, 0.0, 0.0, 0.0);
	glShadeModel (GL_SMOOTH);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	set_callbacks();
	init_camera(&camera);
	init_entities(&world);
	glMatrixMode(GL_MODELVIEW);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
    glLoadIdentity();
    help = load_texture("textures//help.png");

}


int main(int argc, char** argv) {

	glutInit(&argc, argv);
	if (resolution==1){ glutInitWindowSize(1920, 1080);}
	else {glutInitWindowSize(1366, 768);}
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	int wnd = glutCreateWindow("The Earth is not flat");
	glutSetWindow(wnd);
	initialize();

    action.move_forward = FALSE;
    action.move_backward = FALSE;
    action.step_left = FALSE;
    action.step_right = FALSE;
    action.rotate_earth_in_galaxy = TRUE;
    action.call_falcon = FALSE;

	glutMainLoop();
	return 0;
}
