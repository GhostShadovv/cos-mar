#include "glos.h"

#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/GLAux.h>
#include <stdio.h>
#include <stdbool.h>
#include <iostream>
using namespace std;

void myinit(void);
void CALLBACK display(void);
void CALLBACK myReshape(GLsizei w, GLsizei h);

void CALLBACK pune_texturi(void);
void CALLBACK Iarba(void);
void CALLBACK Lemn(float x, float y, float z, float h, float L, float l, string s);
void CALLBACK Scena(void);
void CALLBACK Cos(void);
void CALLBACK Inel(void);
void CALLBACK rot_Y_poz(void);
void CALLBACK rot_Y_neg(void);
void CALLBACK rot_x_p(void);
void CALLBACK rot_x_m(void);
void CALLBACK rot_y_p(void);
void CALLBACK rot_y_m(void);
void CALLBACK movelight_poz(void);
void CALLBACK movelight_neg(void);
void CALLBACK movelight_x_plus(void);
void CALLBACK movelight_x_minus(void);
void CALLBACK toggle_anim(void);
void CALLBACK toggle_anim_m(void);
void CALLBACK Capac_inel(float x, float y, float z);
void CALLBACK Mar(float x, float y, float z);
void CALLBACK Scena_umbre(void);
void CALLBACK Capac_jos(float x, float y, float z);
void CALLBACK Mediu();
void CALLBACK iluminare(void);
void CALLBACK incarca_textura(const char* s);
void CALLBACK calcCoeficientiPlan(float P[3][3], float coef[4]);
void CALLBACK MatriceUmbra(GLfloat puncte[3][3], GLfloat pozSursa[4], GLfloat mat[4][4]);
void CALLBACK animatie(void);