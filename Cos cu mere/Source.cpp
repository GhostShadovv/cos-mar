/*
Programul afiseaza un patrat pe care il translateaza
pe axa x la apasarea sagetilor stanga, dreapta
*/
#include "glos.h"
#include "Header.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/GLAux.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

void myinit(void) {
	glClearColor(1, 1, 1, 1.0);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_DEPTH_TEST);

	pune_texturi();
	iluminare();
}

void CALLBACK display(void) {	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	Scena_umbre();
	auxSwapBuffers();
}

void CALLBACK myReshape(GLsizei w, GLsizei h) {
	if (!h) return;
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(50.0, (GLfloat)w / (GLfloat)h, 1.0, 350.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);  /*  aduce obiectele in volumul de vizualizare   */
}


int main(int argc, char** argv) {
	auxInitDisplayMode(AUX_DOUBLE | AUX_RGB | AUX_DEPTH16);
	auxInitPosition(0, 0, 900, 1000);
	auxInitWindow("Cos cu mere");
	myinit();

	/*aauxKeyFunc(AUX_a, rot_Y_neg);
	auxKeyFunc(AUX_d, rot_Y_poz);
	uxKeyFunc(AUX_q, movelight_poz);
	auxKeyFunc(AUX_e, movelight_neg);*/
	auxKeyFunc(AUX_e, movelight_x_plus);
	auxKeyFunc(AUX_q, movelight_x_minus);
	auxKeyFunc(AUX_DOWN, rot_x_p);
	auxKeyFunc(AUX_UP, rot_x_m);
	auxKeyFunc(AUX_RIGHT, rot_y_p);
	auxKeyFunc(AUX_LEFT, rot_y_m);

	auxKeyFunc(AUX_1, toggle_anim);
	auxKeyFunc(AUX_2, toggle_anim_m);

	auxReshapeFunc(myReshape);
	auxMainLoop(display);
	return(0);
}
