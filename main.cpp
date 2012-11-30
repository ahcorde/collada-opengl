#include <stdio.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

#include <locale.h>

#include <iostream>
#include <pthread.h>

#include "ModelCollada.h"


#define v3f glVertex3f
#define MAXWORLD 30

double width = 500;
double height = 500;

std::vector<ModelCollada*> modelos;


void InitOGL(int w, int h) {
	GLfloat ambient[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat diffuse[] = {1.0, 1.0, 1.0, 1.0};
	GLfloat position[] = {0.0, 3.0, 3.0, 0.0};
	GLfloat lmodel_ambient[] = {0.2, 0.2, 0.2, 1.0};
	GLfloat local_view[] = {0.0};

	glViewport(0, 0, (GLint) w, (GLint) h);
	glDrawBuffer(GL_BACK);
	glClearColor(0.6f, 0.8f, 1.0f, 0.0f);
	glClearDepth(1.0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* With this, the pioneer appears correctly, but the cubes don't */
	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);
	glLightModelfv(GL_LIGHT_MODEL_LOCAL_VIEWER, local_view);
	glEnable(GL_LIGHT0);
	// glEnable (GL_LIGHTING);
	glEnable(GL_POINT_SMOOTH);

	glEnable(GL_TEXTURE_2D); // Enable Texture Mapping
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_NORMALIZE);
	glEnable(GL_DEPTH_TEST); // Enables Depth Testing
	glDepthFunc(GL_LESS);
	glShadeModel(GL_SMOOTH); // Enables Smooth Color Shading
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

// Initialize the rendering objects
void init(int argc, char* argv[]) {

  // Initialize the main window
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(width, height);
  glutCreateWindow("Draw Sphere");
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Configure culling
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
  glFrontFace(GL_CW);

  // Enable depth testing
  glEnable(GL_DEPTH_TEST);
  glDepthMask(GL_TRUE);
  glDepthFunc(GL_LEQUAL);
  glDepthRange(0.0f, 1.0f);

}
float angle = 0;
// Respond to paint events
void display(void) {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  InitOGL(width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  /*Angulo	ratio		znear, zfar*/
  gluPerspective(50.0, width / height, 1.0, 50000.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  /*pos cam		pto central	vector up*/
  gluLookAt( -100, -100, 70,
		  	  0.0, 0.0, 0.0,
		  	  0.0, 0.0, 1.);


  glDisable(GL_TEXTURE_2D);

  glColor3f(0.6, 0.6, 0.6);
  glLineWidth(2.0f);
  glBegin(GL_LINES);

  for (int i = 0; i < ((int) MAXWORLD + 1); i++) {
      v3f(-(int) MAXWORLD * 10 / 2. + (float) i * 10, -(int) MAXWORLD * 10 / 2., 0.);
      v3f(-(int) MAXWORLD * 10 / 2. + (float) i * 10,  (int) MAXWORLD * 10 / 2., 0.);
      v3f(-(int) MAXWORLD * 10 / 2., -(int) MAXWORLD * 10 / 2. + (float) i * 10, 0.);
      v3f( (int) MAXWORLD * 10 / 2., -(int) MAXWORLD * 10 / 2. + (float) i * 10, 0.);
  }
  glEnd();


//  // absolute axis
//  glColor3f(0.7, 0., 0.);
//  glLineWidth(3.0f);
//  glBegin(GL_LINES);
//	  v3f(0.0, 0.0, 0.0);
//	  v3f(10.0, 0.0, 0.0);
//
//  glColor3f(0., 0.7, 0.);
//	  v3f(0.0, 0.0, 0.0);
//	  v3f(0.0, 10.0, 0.0);
//  glColor3f(0., 0., 0.7);
//	  v3f(0.0, 0.0, 0.0);
//	  v3f(0.0, 0.0, 10.0);
//  glEnd();

  //

  glPushMatrix();
  angle+=1;
//  glRotated(angle, 0, 0, 1);
  for(unsigned i = 0; i < modelos.size(); i++){
	  modelos[i]->Draw();
  }
  glPopMatrix();

  glutSwapBuffers();

}

// Respond to reshape events
void reshape(int w, int h) {
   glViewport(0, 0, (GLsizei)w, (GLsizei)h);
   display();
}

void espera()
{
	display();
	usleep(100000);
}

int main(int argc, char* argv[]){


	struct lconv * lc;


    printf ("Locale is: %s\n", setlocale(LC_ALL,"C") );

    lc = localeconv ();
	printf ("Currency symbol is: %s\n-\n",lc->decimal_point);

	// Start GL processing
	init(argc, argv);

//	modelos.push_back( new ModelCollada (std::string("untitled.dae")) );
//	modelos.push_back( new ModelCollada (std::string("tropper/untitled.dae"), std::string("tropper/texturasTropper") ));
//	modelos.push_back( new ModelCollada (std::string("tropper/tropper.dae"), std::string("tropper/texturasTropper") ));

	modelos.push_back( new ModelCollada (std::string("mapa/mapa.dae"), std::string("mapa") ));
//	modelos.push_back( new ModelCollada (std::string("apolo/apolo.dae"), std::string("apolo") ));

	// Set callback functions
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
//	glutIdleFunc( espera );						// register Idle Function


	glutMainLoop();


}
