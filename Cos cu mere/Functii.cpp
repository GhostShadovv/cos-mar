#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/GLAux.h>
#include <stdio.h>
#include <stdbool.h>
#include "Header.h"
#include <math.h>
#include <iostream>
using namespace std;

float	h = 1.5,
		L = 0.5,
		l = 0.2;
GLfloat position[] = { 3, 4, 10, 1.0 };

GLfloat matUmbra[4][4];
boolean umbra = 0;
//oricare 3 pct din plan in sens CCW
GLfloat puncte[3][3] = {
	{ 1.45f, -1.25f, 1.25f },
	{ -1.45f, -1.25f, -1.25f },
	{ -1.45f, -1.25f, 1.25f } };

static GLUquadricObj *qObj;

double anim = 0.00, anim_mar = 0.00, anim_cer = 0.00, anim_iarba = 0.00, anim_deplasare = 0.00, anim_rot_mar=0, anim_deplasare_min=0.20;
GLint y = 0, x = 0, spin = 0;


GLuint IDtextura;
GLuint ID_L1, ID_L2, ID_G1, ID_G2, ID_C, ID_P;
const char* sir;

void CALLBACK Scena(void) {
	glPushMatrix();
		animatie();
		glRotatef(x, 1, 0, 0);
		glRotatef(y, 0, 1, 0);
		//glDepthFunc(GL_EQUAL | GL_LESS);			// !!!
		Cos();
		//glDepthFunc(GL_LESS);						// !!!
		Inel();
		//glDepthFunc(GL_EQUAL | GL_LESS);			// !!!
		Capac_jos(0, 0, 0);
		Capac_inel(0, 1.70, 0);
		glPushMatrix();
			if (anim <= 125) {
				glTranslated(anim_deplasare, anim_mar, 0);
			}
			else {
				glTranslated(anim_deplasare_min, anim_mar, 0);
			}
			glTranslatef(0, 0.2, 0);
			glRotated(anim_rot_mar, 0, 0, -1);
			glTranslatef(0, -0.2, 0);
			Mar(0, 0.2, 0);
		glPopMatrix();
	glPopMatrix();
}
void CALLBACK Scena_umbre(void) {
	MatriceUmbra(puncte, position, matUmbra);
	glPushMatrix();
		glLightfv(GL_LIGHT0, GL_POSITION, position);
		umbra = 0;
		cout.setf(ios::fixed, ios::floatfield);
		cout.precision(2);
		cout.width(8);
		cout << " anim: " << -anim << " |";
		cout.width(9);
		cout << " iarba: " << -anim_iarba << " |";
		cout.width(7);	
		cout << " mar: " << -anim_mar << " |";
		cout.width(10);	
		cout << " rot_mar: " << anim_rot_mar << " |";
		cout.width(6);	
		cout << " cer: " << -anim_cer << " |";
		cout.width(10);	
		cout << " deplasare: " << anim_deplasare << " |";
		cout.width(10);
		cout << " depl_min: " << anim_deplasare_min << endl;
		Scena();
	glPopMatrix();
	glPushMatrix();
		glPushAttrib(GL_LIGHTING_BIT);
		glDisable(GL_LIGHTING);
			glPushMatrix();
				glMultMatrixf((GLfloat*)matUmbra);
				umbra = 1;
				Scena();
			glPopMatrix();
			Mediu();
			//reseteaza starea variabilelor de iluminare
		glPopAttrib();
	glPopMatrix();
}
void CALLBACK Mediu() {
	glPushMatrix();
		glTranslatef(position[0], position[1], position[2]);
		glColor3f(1.0, 0.9, 0);//blue
		auxSolidSphere(0.3);
		glPushMatrix();
			glEnable(GL_TEXTURE_2D);
			glTranslatef(-position[0], -position[1], -position[2]);
			glRotatef(20, 0, -1, 0);
			glRotatef(90, 1, 0, 0);
			glColor3f(0.839, 0.937, 1);
			glRotated(anim_cer, 0, 1, 0);
			qObj = gluNewQuadric();
			glBindTexture(GL_TEXTURE_2D, ID_C);
			gluQuadricTexture(qObj, GL_TRUE);
			gluQuadricNormals(qObj, GLU_SMOOTH);
			gluSphere(qObj, 50, 20, 20);
			gluDeleteQuadric(qObj);
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-position[0], -position[1], -position[2]);
			glColor3f(0.074, 0.603, 0.172);

			glTranslated(-anim_iarba,0, 0);
			glDepthFunc(GL_LESS);
			glBindTexture(GL_TEXTURE_2D, ID_P);
			glBegin(GL_QUADS);
				glTexCoord2f(0, 0);
				glNormal3f(-0.5, -0.2, 0.5);
				glVertex3f(-30, -0.6, -20);		//sj

				glTexCoord2f(1, 0);
				glNormal3f(-0.5, -0.2, 0.5);
				glVertex3f(60, -0.6, -20);	//ss

				glTexCoord2f(1, 1);
				glNormal3f(0.5, 0.2, 0.5f);
				glVertex3f(60, -2, 3);		//ds

				glTexCoord2f(0, 1);
				glNormal3f(0.5, 0.2, 0.5f);
				glVertex3f(-30, -2, 3);	//dj
			glEnd();
		glPopMatrix();
	glPopMatrix();
}
void CALLBACK Inel(void) {
	glPushMatrix();
		float	h_j = 0.2, 
				L_j = 0.85, 
				l_j = 0.1;
						//	1		2	   3   	 4	   5	   6		7	  8	 
		double poz_x_j[8] = { 0.00, 0.70,  1.00,  0.70,  0.00,  -0.70,  -1.00, -0.70 };
		double poz_z_j[8] = { 1.05, 0.73, -0.00, -0.73, -1.05,  -0.73,    0.0,  0.73 };
		double inc_j[8]   = { 0.00, 0.06,  0.01,  0.06,  0.00,   0.06,   0.01,  0.06 };
		double alpha[8] = {    0,   47,    90,   133,   180,    227,    270,   313 };	//rotatie pe y
		double poz_y_j = 0.75;
		for (int i = 0; i < 8; i++) {
			glPushMatrix();
				glTranslatef(poz_x_j[i], -poz_y_j, poz_z_j[i]);
				glRotatef(alpha[i], 0, 1, 0);
				glTranslatef(-poz_x_j[i], poz_y_j, -poz_z_j[i]);
				Lemn(poz_x_j[i], -poz_y_j, poz_z_j[i], h_j, L_j + inc_j[i], l_j, "granit");
			glPopMatrix();
		}
		float	h_m = 0.2,
				L_m = 0.91,
				l_m = 0.1;
							//	1		2	   3   	 4	   5	   6		7	  8	 
		double poz_x_m[8] = { 0.00, 0.77,  1.10,  0.77,  0.00,  -0.77,  -1.10, -0.77 };
		double poz_z_m[8] = { 1.15, 0.80, -0.00, -0.80, -1.15,  -0.80,    0.0,  0.80 };
		double inc_m[8]	  =	{ 0.00, 0.10,  0.01,  0.10,  0.00,   0.10,   0.01,  0.10 };
		double poz_y_m = 0.05;
		for (int i = 0; i < 8; i++) {
			glPushMatrix();
				glTranslatef(poz_x_m[i], -poz_y_m, poz_z_m[i]);
				glRotatef(alpha[i], 0, 1, 0);
				glTranslatef(-poz_x_m[i], poz_y_m, -poz_z_m[i]);
				Lemn(poz_x_m[i], -poz_y_m, poz_z_m[i], h_m, L_m + inc_m[i], l_m, "granit");
			glPopMatrix();
		}
		float	h_s = 0.2,
				L_s = 0.99,
				l_s = 0.31;
							//	1		2	   3   	 4	   5	   6		7	  8	 
		double poz_x_s[8] = { 0.00, 0.77,  1.10,  0.77,  0.00,  -0.77,  -1.10, -0.77 };
		double poz_z_s[8] = { 1.15, 0.80, -0.00, -0.80, -1.15,  -0.80,    0.0,  0.80 };
		double inc_s[8] = { 0.01, 0.10,  0.01,  0.10,  0.01,   0.10,   0.01,  0.10 };
		double poz_y_s = -0.75;
		for (int i = 0; i < 8; i++) {
			glPushMatrix();
				glTranslatef(poz_x_s[i], -poz_y_s, poz_z_s[i]);
				glRotatef(alpha[i], 0, 1, 0);
				glTranslatef(-poz_x_s[i], poz_y_s, -poz_z_s[i]);
				Lemn(poz_x_s[i], -poz_y_s, poz_z_s[i], h_s, L_s + inc_s[i], l_s, "granit");
			glPopMatrix();
		}
	glPopMatrix();
}
void CALLBACK Cos(void) {
	glFrontFace(GL_CW);
	if (umbra == 1) {
		glColor3f(0.15, 0.15, 0.15);
	}
	else {
		glColor3f(1, 1, 1);
	}
	glPushMatrix();
						  //  1		2	     3   	 4	      5	      6		  7	      8		  9		  10	  11	  12   
		double poz_x[12] = { 0.0,	0.5,	0.85,	1.0,	 0.85,	 0.5,	 0.0,	-0.5,	-0.85,	-1.0,	-0.85,	-0.5 };
		double poz_z[12] = { 1.0,	0.9,	0.55,	0.0,	-0.55,	-0.9,	-1.0,	-0.9,	-0.55,	 0.0,	 0.55,	 0.9 };
		double alpha[12] = {   0,	 30,	  60,	 90,	  120,	 150,	 180,	 210,	  240,	 270,	  300,	 330 };	//rotatie pe y
		double beta = 9;	//inclinatie pe x
		for (int i = 0; i < 12; i++) {
			glPushMatrix();
				glTranslatef(poz_x[i], 0, poz_z[i]);
				glRotatef(alpha[i], 0, 1, 0);
				glRotatef(beta, 1, 0, 0);
				glTranslatef(-poz_x[i], 0, -poz_z[i]);
				Lemn(poz_x[i], 0, poz_z[i], h, L, l, "wood");
			glPopMatrix();
		}
		//glPushMatrix();
		//	glTranslatef(0.0, 0, 1.0);
		//	glRotatef(0, 0, 1, 0);
		//	glRotatef(9, 1, 0, 0);
		//	glTranslatef(-0.0, 0, -1.0);

		//	glPushMatrix();
		//		glBindTexture(GL_TEXTURE_2D, ID1);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 1.2*h);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(-0.5, -1.5, 1.2);		//sj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(-0.5, 1.5, 1.2);	//ss

		//			glTexCoord2f(1.2 * L, 0);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(0.5, 1.5, 1.2);		//ds

		//			glTexCoord2f(1.2 * L, 1.2 * h);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(0.5, -1.5, 1.2);	//dj
		//		glEnd();
		//		//dreapta
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 8 * h);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(0.5, -1.5, 1.2);	//dj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(0.5, 1.5, 1.2);		//ds

		//			glTexCoord2f(8 * l, 0);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(0.5, 1.5, 0.8);		//ds

		//			glTexCoord2f(8 * l, 8 * h);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(0.5, -1.5, 0.8);	//dj
		//		glEnd();

		//		//spate
		//		glBindTexture(GL_TEXTURE_2D, ID1);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(-0.5, 1.5, 0.8);	//ss

		//			glTexCoord2f(0, 1.2*h);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(-0.5, -1.5, 0.8);	//sj

		//			glTexCoord2f(1.2 * L, 1.2 * h);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(0.5, -1.5, 0.8);	//dj

		//			glTexCoord2f(1.2 * L, 0);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(0.5, 1.5, 0.8);		//ds
		//		glEnd();
		//		//stanga
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 8 * h);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(-0.5, -1.5, 0.8);	//sj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(-0.5, 1.5, 0.8);	//ss

		//			glTexCoord2f(8 * l, 0);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(-0.5, 1.5, 1.2);		//ss

		//			glTexCoord2f(8 * l, 8 * h);
		//			glNormal3f(-0.5, 0.2, 0.5);
		//			glVertex3f(-0.5, -1.5, 1.2);		//sf
		//		glEnd();

		//		//sus
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, 0.5);
		//			glVertex3f(-0.5, 1.5, 1.2);

		//			glTexCoord2f(1, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(-0.5, 1.5, 0.8);

		//			glTexCoord2f(1, 1);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(0.5, 1.5, 0.8);

		//			glTexCoord2f(0, 1);
		//			glNormal3f(0.5, 0.2, 0.5);
		//			glVertex3f(0.5, 1.5, 1.2);
		//		glEnd();

		//		//jos
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 0);
		//			glNormal3f(0.5, -0.2, 0.5);
		//			glVertex3f(0.5,-1.5, 1.2);

		//			glTexCoord2f(1, 0);
		//			glNormal3f(0.5, -0.2, -0.5);
		//			glVertex3f(0.5, -1.5, 0.8);

		//			glTexCoord2f(1, 1);
		//			glNormal3f(-0.5, -0.2, -0.5);
		//			glVertex3f(-0.5, -1.5, 0.8);

		//			glTexCoord2f(0, 1);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(-0.5, -1.5, 1.2);
		//		glEnd();
		//	glPopMatrix();
		//glPopMatrix();
		//glPushMatrix();
		//	glTranslatef(0.5, 0, 0.9);
		//	glRotatef(30, 0, 1, 0);
		//	glRotatef(9, 1, 0, 0);
		//	glTranslatef(-0.5, 0, -0.9);
		//	glPushMatrix();
		//		glBindTexture(GL_TEXTURE_2D, ID1);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 1.2*h);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(0, -1.5, 1.1);		//sj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(0, 1.5, 1.1);	//ss

		//			glTexCoord2f(1.2 * L, 0);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(1, 1.5, 1.1);		//ds

		//			glTexCoord2f(1.2 * L, 1.2 * h);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(1, -1.5, 1.1);	//dj

		//		glEnd();
		//		//dreapta
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 8 * h);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(1, -1.5, 1.1);	//dj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(0.5, 0.2, 0.5f);
		//			glVertex3f(1, 1.5, 1.1);		//ds

		//			glTexCoord2f(8 * l, 0);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(1, 1.5, 0.7);		//ds

		//			glTexCoord2f(8 * l, 8 * h);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(1, -1.5, 0.7);	//dj
		//		glEnd();

		//		//spate
		//		glBindTexture(GL_TEXTURE_2D, ID1);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(0, 1.5, 0.7);	//ss

		//			glTexCoord2f(0, 1.2*h);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(0, -1.5, 0.7);	//sj

		//			glTexCoord2f(1.2 * L, 1.2 * h);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(1, -1.5, 0.7);	//dj

		//			glTexCoord2f(1.2 * L, 0);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(1, 1.5, 0.7);	//ds
		//		glEnd();


		//		//stanga
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 8 * h);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(0, -1.5, 0.7);	//sj

		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(0, 1.5, 0.7);	//ss

		//			glTexCoord2f(8 * l, 0);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(0, 1.5, 1.1);		//ss

		//			glTexCoord2f(8 * l, 8 * h);
		//			glNormal3f(-0.5, 0.2, 0.5);
		//			glVertex3f(0, -1.5, 1.1);		//sf
		//		glEnd();

		//		//sus
		//		glBindTexture(GL_TEXTURE_2D, ID2);
		//		glBegin(GL_QUADS);
		//			glTexCoord2f(0, 0);
		//			glNormal3f(-0.5, 0.2, 0.5);
		//			glVertex3f(0, 1.5, 1.1);

		//			glTexCoord2f(1, 0);
		//			glNormal3f(-0.5, 0.2, -0.5);
		//			glVertex3f(0, 1.5, 0.7);

		//			glTexCoord2f(1, 1);
		//			glNormal3f(0.5, 0.2, -0.5);
		//			glVertex3f(1, 1.5, 0.7);

		//			glTexCoord2f(0, 1);
		//			glNormal3f(0.5, 0.2, 0.5);
		//			glVertex3f(1, 1.5, 1.1);
		//		glEnd();

		//		//jos
		//		glBegin(GL_QUADS);
		//		glTexCoord2f(0, 0);
		//			glNormal3f(0.5, -0.2, 0.5);
		//			glVertex3f(1, -1.5, 1.1);

		//			glTexCoord2f(1, 0);
		//			glNormal3f(0.5, -0.2, -0.5);
		//			glVertex3f(1, -1.5, 0.7);

		//			glTexCoord2f(1, 1);
		//			glNormal3f(-0.5, -0.2, -0.5);
		//			glVertex3f(0, -1.5, 0.7);

		//			glTexCoord2f(0, 1);
		//			glNormal3f(-0.5, -0.2, 0.5);
		//			glVertex3f(0, -1.5, 1.1);
		//		glEnd();
		//	glPopMatrix();
		//glPopMatrix();
		/*glPushMatrix();
			glTranslatef(0.85, 0, 0.55);
			glRotatef(60, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(-0.85, 0, -0.55);
			Lemn(0.85, 0, 0.55, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(1.0, 0, 0.0);
			glRotatef(90, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(-1.0, 0, -0.0);
			Lemn(1.0, 0, 0.0, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.85, 0, -0.55);
			glRotatef(120, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(-0.85, 0, 0.55);
			Lemn(0.85, 0, -0.55, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.5, 0, -0.9);
			glRotatef(150, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(-0.5, 0, 0.9);
			Lemn(0.5, 0, -0.9, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(0.0, 0, -1.0);
			glRotatef(180, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(-0.0, 0, 1.0);
			Lemn(0.0, 0, -1.0, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, -0.9);
			glRotatef(210, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(0.5, 0, 0.9);
			Lemn(-0.5, 0, -0.9, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.85, 0, -0.55);
			glRotatef(240, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(0.85, 0, 0.55);
			Lemn(-0.85, 0, -0.55, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-1.0, 0, 0.0);
			glRotatef(270, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(1.0, 0, -0.0);
			Lemn(-1.0, 0, 0.0, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.85, 0, 0.55);
			glRotatef(300, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(0.85, 0, -0.55);
			Lemn(-0.85, 0, 0.55, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();
		glPushMatrix();
			glTranslatef(-0.5, 0, 0.9);
			glRotatef(330, 0, 1, 0);
			glRotatef(9, 1, 0, 0);
			glTranslatef(0.5, 0, -0.9);
			Lemn(-0.5, 0, 0.9, 1.5, 0.5, 0.2, "wood");
		glPopMatrix();*/
	glPopMatrix();
}
void CALLBACK iluminare(void) {
	// coeficientii de reflexie pentru reflexia ambientala si cea difuza sunt cei impliciti
	GLfloat mat_ambient[] = { 0.1, 0.1, 0.1, 1.0 };
	GLfloat mat_diffuse[] = { 1, 1, 1, 1.0 };
	/*  reflectanta speculara si exponentul de reflexie speculara nu sunt la valorile implicite (0.0)   */
	GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat mat_shininess[] = { 80.0 };
	// valori implicite pentru intensitatea sursei LIGHT0
	GLfloat light_ambient[] = { 0.2, 0.2, 0.2, 1.0 };
	GLfloat light_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat light_specular[] = { 1.0, 1.0, 1.0, 1.0 };

	GLfloat lmodel_ambient[] = { 0.2, 0.2, 0.2, 1.0 };

	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);

	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);

	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	//permite urmarirea culorilor
	glEnable(GL_COLOR_MATERIAL);
}
void CALLBACK Iarba(void) {
	glBegin(GL_QUADS);
	glColor3d(0.2, 1, 0.05);	//verde

	glNormal3f(0.0, 0.5, 0.2);
	glVertex3f(-5, 0, 0);

	glNormal3f(0.2, 0.5, 0.2);
	glVertex3f(-5, -5, 3);

	glNormal3f(-0.2, 0.5, -0.2);
	glVertex3f(5, -5, 3);

	glNormal3f(-0.2, 0.5, -0.2);
	glVertex3f(5, 0, 0);

	glEnd();
}
void CALLBACK Lemn(float x, float y, float z, float h, float L, float l, string s) {
	L = L / 2;
	l = l / 2;
	h = h / 2;
	glEnable(GL_CULL_FACE);					//activeaza eliminarea fetelor
	glCullFace(GL_BACK);					//sunt eliminate fetele spate
	glEnable(GL_DEPTH_TEST);				//activare test adancime
	if(s=="wood"){
		if (umbra == 1) {
			glColor3f(0.433, 0.388, 0.423);
			glDisable(GL_TEXTURE_2D);
		}
		if (umbra == 0) {
			glColor3f(0.117, 0.580, 0.247);
			glEnable(GL_TEXTURE_2D);
		}
	}
	else {
		if (umbra == 1) {
			glColor3f(0.433, 0.388, 0.423);//gri
			glDisable(GL_TEXTURE_2D);
		}
		if (umbra == 0) {
			glColor3f(0.745, 0.576, 0.576);//granit?
			glEnable(GL_TEXTURE_2D);
		}
	}
	if (s == "wood") {
		glBindTexture(GL_TEXTURE_2D, ID_L1);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, ID_G1);
	}
	//fata
	glBegin(GL_QUADS);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - L, y - h, z + l);		//sj

		glTexCoord2f(0,0);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - L, y + h, z + l);	//ss

		glTexCoord2f(1.2 * L, 0);
		glNormal3f(0.5, 0.2, 0.5f);
		glVertex3f(x + L, y + h, z + l);		//ds

		glTexCoord2f(1.2 * L, 1.2 * h);
		glNormal3f(0.5, 0.2, 0.5f);
		glVertex3f(x + L, y - h, z + l);	//dj

	glEnd();
	//dreapta
	if (s == "wood") {
		glBindTexture(GL_TEXTURE_2D, ID_L2);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, ID_G2);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0, 8 * h);
		glNormal3f(0.5, 0.2, 0.5f);
		glVertex3f(x + L, y - h, z + l);	//dj

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5f);
		glVertex3f(x + L, y + h, z + l);		//ds

		glTexCoord2f(8 * l, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + L, y + h, z - l);		//ds

		glTexCoord2f(8 * l, 8 * h);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + L, y - h, z - l);	//dj
	glEnd();

	//spate
	if (s == "wood") {
		glBindTexture(GL_TEXTURE_2D, ID_L1);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, ID_G1);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - L, y + h, z - l);	//ss

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - L, y - h, z - l);	//sj

		glTexCoord2f(1.2 * L, 1.2 * h);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + L, y - h, z - l);	//dj

		glTexCoord2f(1.2 * L, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + L, y + h, z - l);		//ds
	glEnd();


	//stanga
	if (s == "wood") {
		glBindTexture(GL_TEXTURE_2D, ID_L2);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, ID_G2);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0, 8 * h);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - L, y - h, z - l);	//sj

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - L, y + h, z - l);	//ss

		glTexCoord2f(8 * l, 0);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - L, y + h, z + l);		//ss

		glTexCoord2f(8 * l, 8 * h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - L, y - h, z + l);		//sf
	glEnd();

	//sus
	if (s == "wood") {
		glBindTexture(GL_TEXTURE_2D, ID_L2);
	}
	else {

		glBindTexture(GL_TEXTURE_2D, ID_G2);
	}
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - L, y + h, z + l);

		glTexCoord2f(1, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - L, y + h, z - l);

		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + L, y + h, z - l);

		glTexCoord2f(0, 1);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + L, y + h, z + l);
	glEnd();

	//jos
	glBegin(GL_QUADS);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + L, y - h, z + l);

		glTexCoord2f(1, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + L, y - h, z - l);

		glTexCoord2f(1, 1);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - L, y - h, z - l);

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - L, y - h, z + l);
	glEnd();
}
void CALLBACK Capac_inel(float x, float y, float z) {
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);
	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);//gri
		glDisable(GL_TEXTURE_2D);
	}
	if (umbra == 0) {
		glColor3f(0.745, 0.576, 0.576);//granit?
		glEnable(GL_TEXTURE_2D);
	}
	glBindTexture(GL_TEXTURE_2D, ID_G2);
	//1
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.45, y - 0.849, z + 1.33);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2,0.5);
		glVertex3f(x - 0.53, y - 0.849, z + 1.30);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.42, y - 0.849, z + 0.97);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.35, y - 0.849, z + 1.00);		//4
	glEnd();

	//2
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 1.24, y - 0.849, z + 0.54);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.45, y - 0.849, z + 1.33);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.35, y - 0.849, z + 1.00);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.93, y - 0.849, z + 0.40);		//4
	glEnd();

	//3
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 1.27, y - 0.849, z - 0.47);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 1.24, y - 0.849, z + 0.54);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.93, y - 0.849, z + 0.40);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.95, y - 0.849, z - 0.38);		//4
	glEnd();

	//4
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.55, y - 0.849, z - 1.29);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 1.27, y - 0.849, z - 0.47);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.94, y - 0.849, z - 0.37);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.38, y - 0.849, z - 0.99);		//4
	glEnd();

	//5
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.47, y - 0.849, z - 1.32);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.55, y - 0.849, z - 1.29);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x + 0.38, y - 0.849, z - 0.99);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.35, y - 0.849, z - 1.01);		//4
	glEnd();

	//6
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 1.23, y - 0.849, z - 0.54);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.47, y - 0.849, z - 1.32);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.35, y - 0.849, z - 1.01);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.93, y - 0.849, z - 0.41);		//4
	glEnd();

	//7
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 1.27, y - 0.849, z + 0.47);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 1.23, y - 0.849, z - 0.54);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.93, y - 0.849, z - 0.41);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.95, y - 0.849, z + 0.37);		//4
	glEnd();

	//8
	glBegin(GL_POLYGON);
		glTexCoord2f(1, 1);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.53, y - 0.849, z + 1.30);		//1

		glTexCoord2f(0, 1);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 1.27, y - 0.849, z + 0.47);		//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.95, y - 0.849, z + 0.36);		//3

		glTexCoord2f(1, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.42, y - 0.849, z + 0.97);		//4
	glEnd();
}
void CALLBACK Mar(float x, float y, float z) {
	GLuint ID1 =0, ID2=0;
	glDisable(GL_CULL_FACE);
	glColor3f(0.433, 0.388, 0.4230);//gri
	//textura  
	/*
		const char* sir;
		sir = ".\\granit.bmp";
		incarca_textura(sir);
		ID1 = IDtextura;
		sir = ".\\granit_small.bmp";
		incarca_textura(sir);
		ID2 = IDtextura;
	*/
	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);//gri
		glDisable(GL_TEXTURE_2D);
	}
	if (umbra == 0) {
		glColor3f(0.329, 0.909, 0.450);//verde
		glEnable(GL_TEXTURE_2D);
	}
	//cotor
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.005, y, z + 0.005);		//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.02, y + 0.4, z + 0.02);	//1

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x - 0.005, y, z + 0.005);		//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.02, y + 0.4, z + 0.02);	//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.005, y, z - 0.005);		//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.02, y + 0.4, z - 0.02);	//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.005, y, z - 0.005);		//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.02, y + 0.4, z - 0.02);	//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.005, y, z + 0.005);		//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.02, y + 0.4, z + 0.02);	//4
	glEnd();

	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);//gri
		glDisable(GL_TEXTURE_2D);
	}
	if (umbra == 0) {
		glColor3f(0.450, 0.905, 0.549);//verde
		glEnable(GL_TEXTURE_2D);
	}
	//frunza
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.01, y + 0.1, z);		//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.1, y + 0.3, z + 0.1);	//1

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.4, y + 0.4, z);		//2

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.1, y + 0.3, z - 0.1);	//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.01, y + 0.1, z);		//cc

	glEnd();

	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);//gri
		glDisable(GL_TEXTURE_2D);
	}
	if (umbra == 0) {
		glColor3f(0.984, 0.094, 0.325);//rosu
		glEnable(GL_TEXTURE_2D);
	}
	//sus - tulpina
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z);						//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.20, y + 0.2, z);			//1

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x-0.10, y + 0.2, z - 0.10);		//2

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x , y, z);						//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x, y + 0.2, z-0.15);				//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z);						//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y + 0.20, z - 0.10);	//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.10, y + 0.20, z + 0.10);	//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z);						//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x, y + 0.20, z + 0.20);			//6

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.20, y + 0.20, z);			//7
	glEnd();

	//sus
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.20);			//6

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.30);			//s2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.20, y + 0.20, z);			//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.30, y + 0.20, z + 0.20);	//s4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.10, y + 0.20, z - 0.10);	//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.40, y + 0.20, z - 0.10);	//s6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x, y + 0.20, z - 0.15);			//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x - 0.20, y + 0.20, z - 0.30);	//s8

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x, y + 0.20, z - 0.15);			//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.10, y + 0.20, z - 0.40);	//s8

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y + 0.20, z - 0.10);	//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.35, y + 0.20, z - 0.25);	//s10

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.10, y + 0.20, z + 0.10);	//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.40, y + 0.20, z + 0.20);	//s12

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.20);			//6

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.30);			//s14
	glEnd();

	//sus - lateral
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.30);			//s2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z + 0.50);					//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.30, y + 0.20, z + 0.20);	//s4

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.40, y, z + 0.40);			//2
			
		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.40, y + 0.20, z - 0.10);	//s6

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.60, y, z + 0.10);			//3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.20, y + 0.20, z - 0.30);	//s8

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.40, y, z-0.40);			//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.10, y + 0.20, z - 0.40);	//s8.1

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y, z - 0.60);			//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.35, y + 0.20, z - 0.25);	//s10

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.45, y, z - 0.35);			//6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.40, y + 0.20, z + 0.20);	//s12

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.50, y, z + 0.25);			//7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y + 0.20, z + 0.30);			//s14

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z + 0.50);					//1
	glEnd();

	//mijloc-sus
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z + 0.50);					//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.20, z + 0.60);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.40, y, z + 0.40);			//2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.20, z + 0.50);	//m2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, 0.2, 0.5);
		glVertex3f(x - 0.60, y, z + 0.10);			//3

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.70, y - 0.20, z + 0.10);	//m3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, 0.2, -0.5);
		glVertex3f(x - 0.40, y, z - 0.40);			//4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.45, y - 0.20, z - 0.45);	//m4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y, z - 0.60);			//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.20, z - 0.65);	//m5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.45, y, z - 0.35);			//6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.50, y - 0.20, z - 0.40);	//m6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x + 0.50, y, z + 0.25);			//7

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.60, y - 0.20, z + 0.25);	//m7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, 0.2, 0.5);
		glVertex3f(x, y, z + 0.50);					//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.20, z + 0.60);			//m1
	glEnd();

	//mijloc
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.20, z + 0.60);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.50, z + 0.60);			//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.20, z + 0.50);	//m2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.50, z + 0.50);	//2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.70, y - 0.20, z + 0.10);	//m3

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.70, y - 0.50, z + 0.10);	//3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.45, y - 0.20, z - 0.45);	//m4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.45, y - 0.50, z - 0.45);	//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.20, z - 0.65);	//m5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.50, z - 0.65);	//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.50, y - 0.20, z - 0.40);	//m6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.50, y - 0.50, z - 0.40);	//6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.60, y - 0.20, z + 0.25);	//m7

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.60, y - 0.50, z + 0.25);	//7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.20, z + 0.60);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.50, z + 0.60);			//1
	glEnd();

	//mijloc-jos
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.50, z + 0.60);			//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.70, z + 0.50);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.50, z + 0.50);	//2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.70, z + 0.40);	//m2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.70, y - 0.50, z + 0.10);	//3

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.60, y - 0.70, z + 0.10);	//m3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.45, y - 0.50, z - 0.45);	//4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.40, y - 0.70, z - 0.40);	//m4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.50, z - 0.65);	//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.70, z - 0.60);	//m5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.50, y - 0.50, z - 0.40);	//6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.45, y - 0.70, z - 0.35);	//m6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.60, y - 0.50, z + 0.25);	//7

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.50, y - 0.70, z + 0.25);	//m7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.50, z + 0.60);			//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.70, z + 0.50);			//m1
	glEnd();

	//jos-lateral
	glBindTexture(GL_TEXTURE_2D, ID1);
		glBegin(GL_QUAD_STRIP);
		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.70, z + 0.50);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.30);			//s1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.40, y - 0.70, z + 0.40);	//m2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.30, y - 0.90, z + 0.20);	//s2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.60, y - 0.70, z + 0.10);	//m3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.40, y - 0.90, z - 0.10);	//s3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.40, y - 0.70, z - 0.40);	//m4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.20, y - 0.90, z - 0.30);	//s4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, 0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.70, z - 0.60);	//m5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.10, y - 0.90, z - 0.40);	//s5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.45, y - 0.70, z - 0.35);	//m6

		glTexCoord2f(0, 0);
		glNormal3f(0.5,- 0.2, -0.5);
		glVertex3f(x + 0.35, y - 0.90, z - 0.25);	//s6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.50, y - 0.70, z + 0.25);	//m7

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.40, y - 0.90, z + 0.20);	//s7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.70, z + 0.50);			//m1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.30);			//s1
	glEnd();

	//jos
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_QUAD_STRIP);

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.30);			//s1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.20);			//1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.30, y - 0.90, z + 0.20);	//s2

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.20, y - 0.90, z);			//2

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.40, y - 0.90, z - 0.10);	//s3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.10, y - 0.90, z - 0.10);	//3

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.20, y - 0.90, z - 0.30);	//s4

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x, y - 0.90, z - 0.15);			//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.10, y - 0.90, z - 0.40);	//s5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x, y - 0.90, z - 0.15);			//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.35, y - 0.90, z - 0.25);	//s6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.90, z - 0.10);	//6

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.40, y - 0.90, z + 0.20);	//s7

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.10, y - 0.90, z + 0.10);	//7

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.30);			//s1

		glTexCoord2f(0, 1.2*h);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.20);			//s1
	glEnd();

	//jos - tulpina
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.80, z);					//cc

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.20, y - 0.90, z);			//1

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.10, y - 0.90, z - 0.10);	//2

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.80, z);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x, y - 0.90, z - 0.15);			//3

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.80, z);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x + 0.20, y - 0.90, z - 0.10);	//4

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.10, y - 0.90, z + 0.10);	//5

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.80, z);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z + 0.20);			//6

		glTexCoord2f(0, 1.2*h);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.20, y - 0.90, z);			//7
	glEnd();

	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);
		glDisable(GL_TEXTURE_2D);
	}
	if (umbra == 0) {
		glColor3f(0.101, 0.380, 0.156);
		glEnable(GL_TEXTURE_2D);
	}
	//jos - cotor
	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x - 0.10, y - 0.80, z + 0.10);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.10, y - 0.80, z - 0.10);		//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z);		//cc
	glEnd();

	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.10, y - 0.80, z + 0.10);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, -0.5);
		glVertex3f(x - 0.10, y - 0.80, z - 0.10);		//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z);		//cc
	glEnd();

	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x + 0.10, y - 0.80, z);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(-0.5, -0.2, 0.5);
		glVertex3f(x - 0.10, y - 0.80, z);		//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z);		//cc
	glEnd();

	glBindTexture(GL_TEXTURE_2D, ID1);
	glBegin(GL_TRIANGLE_STRIP);
		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, -0.5);
		glVertex3f(x, y - 0.80, z - 0.10);					//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.80, z + 0.10);		//cc

		glTexCoord2f(0, 0);
		glNormal3f(0.5, -0.2, 0.5);
		glVertex3f(x, y - 0.90, z);		//cc
	glEnd();
	glEnable(GL_CULL_FACE);
}
void CALLBACK Capac_jos(float x, float y, float z) {
	glRotatef(x, 1, 0, 0);
	glRotatef(y, 0, 1, 0);

	glDisable(GL_CULL_FACE);
	if (umbra == 1) {
		glColor3f(0.433, 0.388, 0.423);//gri
	}
	if (umbra == 0) {
		glColor3f(0.745, 0.576, 0.576);//granit?
	}
		//capac jos
		glBindTexture(GL_TEXTURE_2D, ID_G1);
		glBegin(GL_POLYGON);
			glTexCoord2f(0.66, 0);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x + 0.42, y - 0.851, z + 1.08);		//1

			glTexCoord2f(1, 0.33);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x + 1.05, y - 0.851, z + 0.43);		//2

			glTexCoord2f(1, 0.66);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x + 1.05, y - 0.851, z - 0.43);		//3

			glTexCoord2f(0.66, 1);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x + 0.42, y - 0.851, z -1.08);		//4

			glTexCoord2f(0.33, 1);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x - 0.42, y - 0.851, z - 1.08);		//5

			glTexCoord2f(0, 0.66);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x - 1.05, y - 0.851, z - 0.43);		//6

			glTexCoord2f(0, 0.33);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x - 1.05, y - 0.851, z + 0.43);		//7

			glTexCoord2f(0.33, 0);
			glNormal3f(-0.5, -0.2, 0.5);
			glVertex3f(x - 0.42, y - 0.851, z + 1.08);		//8
		glEnd();
	glEnable(GL_CULL_FACE);
}
void CALLBACK incarca_textura(const char* s){
	AUX_RGBImageRec *pImagineTextura = auxDIBImageLoad(s);
	if (pImagineTextura != NULL)	{
		glGenTextures(1, &IDtextura);

		glBindTexture(GL_TEXTURE_2D, IDtextura);
		glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);		
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		glTexImage2D(GL_TEXTURE_2D, 0, 3, pImagineTextura->sizeX, pImagineTextura->sizeY,
			0, GL_RGB, GL_UNSIGNED_BYTE, pImagineTextura->data);
			// GL_RGB8
	}
	if (pImagineTextura) {
		if (pImagineTextura->data) {
			free(pImagineTextura->data);
		}
		free(pImagineTextura);
	}
}
void CALLBACK pune_texturi(void) {
	sir = ".\\granit.bmp";
	incarca_textura(sir);
	ID_G1 = IDtextura;

	sir = ".\\granit_small.bmp";
	incarca_textura(sir);
	ID_G2 = IDtextura;

	sir = ".\\wood.bmp";
	incarca_textura(sir);
	ID_L1 = IDtextura;

	sir = ".\\wood_small.bmp";
	incarca_textura(sir);
	ID_L2 = IDtextura;

	sir = ".\\cer.bmp";
	incarca_textura(sir);
	ID_C = IDtextura;

	sir = ".\\iarba.bmp";
	incarca_textura(sir);
	ID_P = IDtextura;
}
void CALLBACK calcCoeficientiPlan(float P[3][3], float coef[4]) {
	float v1[3], v2[3];
	float length;
	static const int x = 0, y = 1, z = 2;

	// calculeaza 2 vectori din 3 pct
	v1[x] = P[0][x] - P[1][x];
	v1[y] = P[0][y] - P[1][y];
	v1[z] = P[0][z] - P[1][z];
	v2[x] = P[1][x] - P[2][x];
	v2[y] = P[1][y] - P[2][y];
	v2[z] = P[1][z] - P[2][z];

	//se calc produsul vectorial al celor 2 vectori => al3lea vector cu componentele A,B,C chiar coef din ec. planului
	coef[x] = v1[y] * v2[z] - v1[z] * v2[y];
	coef[y] = v1[z] * v2[x] - v1[x] * v2[z];
	coef[z] = v1[x] * v2[y] - v1[y] - v2[x];

	//normalizare vector
	length = (float)sqrt(coef[x] * coef[x] + coef[y] * coef[y] + coef[z] * coef[z]);
	coef[x] /= length;
	coef[y] /= length;
	coef[z] /= length;
}
void CALLBACK MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]) {
	// creeaza matricea care da umbra cunoscandu-se coef planului + poz sursei
	// IN mat SE SALVEAZA MATRICEA

	GLfloat coefPlan[4], temp;

	//determina coef planului
	calcCoeficientiPlan(puncte, coefPlan);

	// determinam D
	coefPlan[3] = -(coefPlan[0] * puncte[2][0] + coefPlan[1] * puncte[2][1] + coefPlan[2] * puncte[2][2]);

	// temp= AxL + ByL + CzL + Dw
	temp = coefPlan[0] * pozSursa[0] + coefPlan[1] * pozSursa[1] + coefPlan[2] * pozSursa[2] + coefPlan[3] * pozSursa[3];

	//prima coloana
	mat[0][0] = temp - coefPlan[0] * pozSursa[0];
	mat[1][0] = 0.0f - coefPlan[1] * pozSursa[0];
	mat[2][0] = 0.0f - coefPlan[2] * pozSursa[0];
	mat[3][0] = 0.0f - coefPlan[3] * pozSursa[0];
	//a 2a coloana
	mat[0][1] = 0.0f - coefPlan[0] * pozSursa[1];
	mat[1][1] = temp - coefPlan[1] * pozSursa[1];
	mat[2][1] = 0.0f - coefPlan[2] * pozSursa[1];
	mat[3][1] = 0.0f - coefPlan[3] * pozSursa[1];
	//a 3a coloana
	mat[0][2] = 0.0f - coefPlan[0] * pozSursa[2];
	mat[1][2] = 0.0f - coefPlan[1] * pozSursa[2];
	mat[2][2] = temp - coefPlan[2] * pozSursa[2];
	mat[3][2] = 0.0f - coefPlan[3] * pozSursa[2];
	//a4a coloana
	mat[0][3] = 0.0f - coefPlan[0] * pozSursa[3];
	mat[1][3] = 0.0f - coefPlan[1] * pozSursa[3];
	mat[2][3] = 0.0f - coefPlan[2] * pozSursa[3];
	mat[3][3] = temp - coefPlan[3] * pozSursa[3];
}

void CALLBACK animatie(void) {
	if (anim > 90) {
		glTranslatef(-anim_deplasare, 0, 0);
	}
	if (anim <= 90) {
		glTranslated(-anim_iarba, 0, 0);
		glRotatef(-anim, 0, 0, 1);
	}
	else {
		glTranslated(-anim_iarba, 0, 0);
		glRotatef(-90, 0, 0, 1);
	}
}
void CALLBACK toggle_anim(void) {
	if (anim < 0) {
		anim = 0;
	}
	if (anim > 0 && anim <= 90) {
		anim_cer += 0.04;
		anim_mar += .02;
		anim_iarba += .03;
	}
	if (anim >= 90 && anim <= 110) {
		anim_mar += .1;
		anim_cer += 0.05;
		anim_iarba += .07;
		anim_deplasare += 0.04;
		anim_rot_mar += 1;
	}
	if (anim >= 110 && anim <= 125){
		anim_mar += .03;
		anim_cer += 0.05;
		anim_iarba += .07;
		anim_deplasare -= 0.04;
		anim_rot_mar += 5;
	}
	if (anim >= 125 && anim <= 127) {
		anim_mar += .03;
		anim_cer += 0.05;
		anim_iarba += .07;
		anim_deplasare_min += 0.04;
		anim_rot_mar += 5;
	}	
	if (anim >= 127 && anim <= 163) {
		anim_mar += .03;
		anim_cer += 0.02;
		anim_iarba += .01;
		anim_rot_mar += 5;
	}
	if (anim >= 163 && anim < 170) {
		anim_mar -= .03;
		anim_cer -= 0.02;
		anim_iarba -= .01;
		anim_rot_mar -= 5;
	}
	if (anim >= 170) {
		anim_mar = 5.39;
		anim_cer -= 0.02;
		anim_iarba = 5.83;
		anim_rot_mar = 266;
	}
	anim += 1;
}
void CALLBACK toggle_anim_m(void) {
	anim -= 1;
	if (anim < 0) {
		anim = 0;
	}
	if (anim > 0 && anim <= 90) {
		anim_cer -= 0.04;
		anim_mar -= .02;
		anim_iarba -= .03;
	}
	if (anim >= 90 && anim <= 110) {
		anim_mar -= .1;
		anim_cer -= 0.05;
		anim_iarba -= .07;
		anim_deplasare -= 0.04;
		anim_rot_mar -= 1;
	}
	if (anim >= 110 && anim <= 125) {
		anim_mar -= .03;
		anim_cer -= 0.05;
		anim_iarba -= .07;
		anim_deplasare += 0.04;
		anim_rot_mar -= 5;
	}
	if (anim >= 125 && anim <= 127) {
		anim_mar -= .03;
		anim_cer -= 0.05;
		anim_iarba -= .07;
		anim_deplasare_min -= 0.04;
		anim_rot_mar -= 5;
	}
	if (anim >= 127 && anim <= 163) {
		anim_mar -= .03;
		anim_cer -= 0.05;
		anim_iarba -= .01;
		//anim_deplasare_min -= 0.04;
		anim_rot_mar -= 5;
	}
	if (anim >= 163 && anim < 170) {
		anim_mar += .03;
		anim_cer += 0.02;
		anim_iarba += .01;
		anim_rot_mar += 5;
	}
	if (anim >= 170) {
		anim_mar = 5.39;
		anim_cer += 0.02;
		anim_iarba = 5.83;
		anim_rot_mar = 266;
	}
}
void CALLBACK rot_Y_poz(void) {
	y += 10;
}
void CALLBACK rot_Y_neg(void) {
	y -= 10;
}
void CALLBACK movelight_poz(void) {
	spin = (spin + 10) % 360;
}
void CALLBACK movelight_neg(void) {
	spin = (spin - 10) % 360;
}
void CALLBACK rot_x_p(void) {
	x = (x + 10) % 360;
}
void CALLBACK rot_x_m(void) {
	x = (x - 10) % 360;
}
void CALLBACK rot_y_p(void) {
	y = (y + 10) % 360;
}
void CALLBACK rot_y_m(void) {
	y = (y - 10) % 360;
}
void CALLBACK movelight_x_plus(void) {
	position[0] += 1;
}
void CALLBACK movelight_x_minus(void) {
	position[0] -= 1;
}
void old(void) {
	//lemn
	//glBegin(GL_QUAD_STRIP);
	//	glTexCoord2f(0, 2*h);
	//	glNormal3f(-0.5, -0.2, 0.5);
	//	glVertex3f(x - L, y - h, z + l);		//sj

	//	glTexCoord2f(0,0);
	//	glNormal3f(-0.5, -0.2, 0.5);
	//	glVertex3f(x - L, y + h, z + l);	//ss

	//	//glColor3d(1, 0, 0);
	//	glTexCoord2f(2 * L, 2 * h);
	//	glNormal3f(0.5, 0.2, 0.5f);
	//	glVertex3f(x + L, y - h, z + l);	//dj

	//	glTexCoord2f(2 * L, 0);
	//	glNormal3f(0.5, 0.2, 0.5f);
	//	glVertex3f(x + L, y + h, z + l);		//ds

	//	//glColor3d(0.9, 0, 0);
	//	glTexCoord2f(3 * l, 3 * h);
	//	glNormal3f(0.5, 0.2, -0.5);
	//	glVertex3f(x + L, y - h, z - l);	//dj

	//	glTexCoord2f(3 * l, 0);
	//	glNormal3f(0.5, 0.2, -0.5);
	//	glVertex3f(x + L, y + h, z - l);		//ds

	//	//glColor3f(0.8, 0, 0);

	//	glTexCoord2f(5 * L, 5 * h);
	//	glNormal3f(-0.5, 0.2, -0.5);
	//	glVertex3f(x - L, y - h, z - l);	//sj

	//	glTexCoord2f(5 * L, 0);
	//	glNormal3f(-0.5, 0.2, -0.5);
	//	glVertex3f(x - L, y + h, z - l);	//ss

	//	//glColor3f(0.9, 0, 0);
	//	glTexCoord2f(6 * l, 6 * h);
	//	glNormal3f(-0.5, 0.2, 0.5);
	//	glVertex3f(x - L, y - h, z + l);		//sf
	//	glTexCoord2f(6 * l, 0);
	//	glNormal3f(-0.5, -0.2, 0.5);
	//	glVertex3f(x - L, y + h, z + l);		//ss
	//glEnd();

	//inel
	////1
	//Lemn(0.00, -0.75, 1.05, h, L, l);
	////2
	//glPushMatrix();
	//	glTranslatef(0.70, -0.75, 0.73);
	//	glRotatef(47, 0, 1, 0);
	//	glTranslatef(-0.70, 0.75, -0.73);
	//	Lemn(0.70, -0.75, 0.73, h, L + 0.06, l);
	//glPopMatrix();
	////3
	//glPushMatrix();
	//	glTranslatef(1.00, -0.75, 0);
	//	glRotatef(90, 0, 1, 0);
	//	glTranslatef(-1.00, 0.75, -0);
	//	Lemn(1.00, -0.75, 0, h, L+0.01, l);
	//glPopMatrix();
	////4
	//glPushMatrix();
	//	glTranslatef(0.70, -0.75, -0.73);
	//	glRotatef(133, 0, 1, 0);
	//	glTranslatef(-0.70, 0.75, 0.73);
	//	Lemn(0.70, -0.75, -0.73, h, L + 0.06, l);
	//glPopMatrix();
	////5
	//Lemn(0.00, -0.75, -1.05, h, L, l);
	////6
	//glPushMatrix();
	//	glTranslatef(-0.70, -0.75, -0.73);
	//	glRotatef(227, 0, 1, 0);
	//	glTranslatef(0.70, 0.75, 0.73);
	//	Lemn(-0.70, -0.75, -0.73, h, L + 0.06, l);
	//glPopMatrix();
	////7
	//glPushMatrix();
	//	glTranslatef(-1.00, -0.75, 0);
	//	glRotatef(270, 0, 1, 0);
	//	glTranslatef(1.00, 0.75, -0);
	//	Lemn(-1.00, -0.75, 0, h, L+0.01, l);
	//glPopMatrix();
	////8
	//glPushMatrix();
	//	glTranslatef(-0.70, -0.75, 0.73);
	//	glRotatef(313, 0, 1, 0);
	//	glTranslatef(0.70, 0.75, -0.73);
	//	Lemn(-0.70, -0.75, 0.73, h, L + 0.06, l);
	//glPopMatrix();
}