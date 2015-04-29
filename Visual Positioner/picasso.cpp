#include "picasso.h"

void draw(ARdouble trans[3][4])
{
	ARdouble  gl_para[16];
	GLfloat   mat_diffuse[] = { 0.0f, 0.0f, 1.0f, 0.0f };
	GLfloat   mat_flash[] = { 1.0f, 1.0f, 1.0f, 0.0f };
	GLfloat   mat_flash_shiny[] = { 50.0f };
	GLfloat   light_position[] = { 100.0f, -200.0f, 200.0f, 0.0f };
	GLfloat   light_ambi[] = { 0.1f, 0.1f, 0.1f, 0.0f };
	GLfloat   light_color[] = { 1.0f, 1.0f, 1.0f, 0.0f };

	glPushMatrix();

	argDrawMode3D(vp);
	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	/* load the camera transformation matrix */
	argConvGlpara(trans, gl_para);
	glMatrixMode(GL_MODELVIEW);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(gl_para);
#else
	glLoadMatrixd(gl_para);
#endif

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambi);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_color);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_color);
	glMaterialfv(GL_FRONT, GL_SPECULAR, mat_flash);
	glMaterialfv(GL_FRONT, GL_SHININESS, mat_flash_shiny);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, mat_diffuse);
	glMaterialfv(GL_FRONT, GL_AMBIENT, mat_diffuse);

#if 1
	glTranslatef(0.0f, 0.0f, 40.0f);
	glutSolidCube(80.0);
#else
	glTranslatef(0.0f, 0.0f, 20.0f);
	glRotatef(90.0f, 1.0f, 0.0f, 0.0f);
	glutSolidTeapot(40.0);
#endif
	glDisable(GL_LIGHT0);
	glDisable(GL_LIGHTING);

	glDisable(GL_DEPTH_TEST);

	argDrawMode2D(vp);
	glPopMatrix();
}


void showId(marker &mark) {
	ARdouble  gl_para[16];

	glPushMatrix();
	argDrawMode3D(vp);

	glClearDepth(1.0);
	glClear(GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LEQUAL);
	/* load the camera transformation matrix */
	argConvGlpara(mark.marker_trans, gl_para);
	glMatrixMode(GL_MODELVIEW);
#ifdef ARDOUBLE_IS_FLOAT
	glLoadMatrixf(gl_para);
#else
	glLoadMatrixd(gl_para);
#endif

	char tmp[256];
	sprintf(tmp, "%d,%d", mark.id, mark.idx);
	if (mark.measurements > SAMPLES || runMode == RUN_MODE_POSITIONER)
		glColor3f(0.0f, 1.0f, 0.0f);
	else
		glColor3f(0.0f, 1.0f, 1.0f);
	argDrawStringsByIdealPos(tmp, 0, 0);

	glDisable(GL_DEPTH_TEST);

	argDrawMode2D(vp);
	glPopMatrix();
}