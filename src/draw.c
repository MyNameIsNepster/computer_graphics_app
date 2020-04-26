#include "draw.h"
#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include "camera.h"


void draw_triangles(const struct Model* model)
{
	int i, k;
	int vertex_index, normal_index;
	double x, y, z, normal_x, normal_y, normal_z;

	glBegin(GL_TRIANGLES);

	for (i = 0; i < model->n_triangles; ++i) {
		for (k = 0; k < 3; ++k) {
			normal_index = model->triangles[i].points[k].normal_index;
			normal_x = model->normals[normal_index].x;
			normal_y = model->normals[normal_index].y;
			normal_z = model->normals[normal_index].z;
			glNormal3d(normal_x, normal_y, normal_z);
			vertex_index = model->triangles[i].points[k].vertex_index;
			x = model->vertices[vertex_index].x;
			y = model->vertices[vertex_index].y;
			z = model->vertices[vertex_index].z;
            switch(k){
            case 0:
                glTexCoord2f(0, 0);
                break;
            case 1:
                glTexCoord2f(0.1*z, 0);
                break;
            case 2:
                glTexCoord2f(0, 0.01);
                break;
            }
			glVertex3d(x, y, z);

		}
	}

	glEnd();
}


void draw_quads(const struct Model* model)
{
    int i, k;
    int vertex_index, texture_index;
    double x, y, z, u, v;

	glBegin(GL_QUADS);

    for (i = 0; i < model->n_quads; ++i) {
        for (k = 0; k < 4; ++k) {

            texture_index = model->quads[i].points[k].texture_index;
            u = model->texture_vertices[texture_index].u;
            v = model->texture_vertices[texture_index].v;
            glTexCoord2f(u, 1-v);

            vertex_index = model->quads[i].points[k].vertex_index;
            x = model->vertices[vertex_index].x;
            y = model->vertices[vertex_index].y;
            z = model->vertices[vertex_index].z;
            glVertex3d(x, y, z);
        }
    }

    glEnd();
}



void draw_normals(const struct Model* model, double length)
{
	int i;
	double x1, y1, z1, x2, y2, z2;

	glColor3f(0, 0, 1);

	glBegin(GL_LINES);

	for (i = 0; i < model->n_vertices; ++i) {
		x1 = model->vertices[i].x;
		y1 = model->vertices[i].y;
		z1 = model->vertices[i].z;
		x2 = x1 + model->normals[i].x * length;
		y2 = y1 + model->normals[i].y * length;
		z2 = z1 + model->normals[i].z * length;
		glVertex3d(x1, y1, z1);
		glVertex3d(x2, y2, z2);
	}
	glEnd();
}



void draw_model(const struct Model* model)
{
	draw_triangles(model);
	draw_quads(model);
}


void draw_skybox_bottom(Entity skybox)
{
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 12;
    n_stacks = 6;
    radius = SKYBOX_SIZE;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(radius, radius, radius);

   /* GLfloat light_ambient[] = { 5,5,5, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);*/

    glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double)i / n_stacks;
        v2 = (double)(i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double)k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, -z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, -z2);
        }
    }

    glEnd();
    glPopMatrix();
}



void draw_skybox_top(Entity skybox)
{
    double theta, phi1, phi2;
    double x1, y1, z1;
    double x2, y2, z2;
    double u, v1, v2;

    int n_slices, n_stacks;
    double radius;
    int i, k;

    n_slices = 12;
    n_stacks = 6;
    radius = SKYBOX_SIZE;

    glPushMatrix();
    glBindTexture(GL_TEXTURE_2D, skybox.texture);
    glScaled(radius, radius, radius);

   /* GLfloat light_ambient[] = { 5,5,5, 1.0 };
    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, light_ambient);*/

    glColor3f(1, 1, 1);

	glBegin(GL_TRIANGLE_STRIP);

    for (i = 0; i < n_stacks; ++i) {
        v1 = (double)i / n_stacks;
        v2 = (double)(i + 1) / n_stacks;
        phi1 = v1 * M_PI / 2.0;
        phi2 = v2 * M_PI / 2.0;
        for (k = 0; k <= n_slices; ++k) {
            u = (double)k / n_slices;
            theta = u * 2.0 * M_PI;
            x1 = cos(theta) * cos(phi1);
            y1 = sin(theta) * cos(phi1);
            z1 = sin(phi1);
            x2 = cos(theta) * cos(phi2);
            y2 = sin(theta) * cos(phi2);
            z2 = sin(phi2);
            glTexCoord2d(u, 1.0 - v1);
            glVertex3d(x1, y1, z1);
            glTexCoord2d(u, 1.0 - v2);
            glVertex3d(x2, y2, z2);
        }
    }

    glEnd();
    glPopMatrix();
}



void draw_environment(World world, Rotate* rotate, Move move ){

	glEnable(GL_TEXTURE_2D);


	//Draw the bottom skybox.
	draw_skybox_bottom(world.skybox);


	//Draw the top skybox.
	draw_skybox_top(world.skybox);


//	glDisable(GL_TEXTURE_2D);


	//Draw the sun.
	glPushMatrix();
		glTranslatef(0,0,0);
		glBindTexture(GL_TEXTURE_2D, world.sun.texture);
		glScalef(1.0f, 1.0f, 1.0f);
		glRotatef(rotate->sun_rotation, 1, 0, 0);
		glRotatef(90, 0, 1, 0);
		draw_model(&world.sun.model);
//		glutSolidSphere (100.0, 60, 60);
    glPopMatrix();


    //Draw the earth.
	  glPushMatrix();
    	glTranslatef (move.earth.x,move.earth.y,move.earth.z);
    	glBindTexture(GL_TEXTURE_2D, world.earth.texture);
    	glScalef(1.0f, 1.0f, 1.0f);
		glRotatef(rotate->earth_rotation, 0, 0, 1);
    	draw_model(&world.earth.model);
//		glutSolidSphere (100.0, 60, 60);
    glPopMatrix();


     //Draw the deathstar.
    glPushMatrix();
    	glTranslatef (move.earth.x+1000,move.earth.y+1000,move.earth.z-100);
    	glBindTexture(GL_TEXTURE_2D, world.deathstar.texture);
    	glScalef(1.0f, 1.0f, 1.0f);
		glRotatef(180, 0, 0, 1);
    	draw_model(&world.deathstar.model);
    glPopMatrix();


     //Draw the falcon.
    glPushMatrix();
        glTranslatef(move.falcon.x,move.falcon.y,move.falcon.z);
        glBindTexture(GL_TEXTURE_2D, world.falcon.texture);
    	glScalef(1.0f, 1.0f, 1.0f);
        glRotatef(90, 1, 0, 0);
        glRotatef(270, 0, 1, 0);
        draw_model(&world.falcon.model);
    glPopMatrix();



}
