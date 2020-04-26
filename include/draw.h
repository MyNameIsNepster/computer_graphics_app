#ifndef DRAW_H
#define DRAW_H
#include "model.h"
#define SKYBOX_SIZE 10000.0

typedef struct Position
{
	double x;
	double y;
	double z;
}Position;

typedef struct {

	double earth_rotation;
	double sun_rotation;
}Rotate;

typedef struct{
	Position earth;
	Position sun;
	Position falcon;
}Move;


//Draw the model.
void draw_model(const struct Model* model);


//Draw the triangles of the model.
void draw_triangles(const struct Model* model);


//Draw the quads of the model.
void draw_quads(const struct Model* model);


//Draw the normal vectors of the model.
void draw_normals(const struct Model* model, double length);


//Draw the top skybox.
void draw_skybox_top(Entity skybox);


//Draw the bottom skybox.
void draw_skybox_bottom(Entity skybox);


//Draw the entitys to the world.
void draw_environment(World world,Rotate* rotate, Move move);


#endif
