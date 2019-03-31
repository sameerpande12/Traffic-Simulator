#include <any>
#include <cstring>
#include <string>
#include <iostream>
#include <map>
#include <vector>
#include <random>
#include <queue>
#include <fstream>
#include <sstream>
#include <chrono>
#include <thread>
#include <GL/glut.h> //for linux
//#include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h for mac
//#include <GLUT/glew.h>
//#include <GLFW/glfw3.h>
//include <glad/glad.h>


#include <stdlib.h>
#include <math.h>
#include <iostream>


#include "draw.h"
#include "processKeys.h"
#include "common.h"
#include "Road.h"
#include "template.h"
#include "Vehicle.h"
#include "updateCreatePrint.h"


#define PI 3.14159
#define GL_SILENCE_DEPRECATION

using namespace std;

void changeSize(int w, int h) {

   // Prevent a divide by zero, when window is too short
   // (you cant make a window of zero width).
   if (h == 0)
      h = 1;

   float ratio =  w * 1.0 / h;

   // Use the Projection Matrix
   glMatrixMode(GL_PROJECTION);

   // Reset Matrix
   glLoadIdentity();

   // Set the viewport to be the entire window
   glViewport(0, 0, w, h);

   // Set the correct perspective.
   gluPerspective(perspective_angle, ratio*1.0, 10.0f, 100.0f);

   // Get Back to the Modelview
   glMatrixMode(GL_MODELVIEW);
}

void draw_cylinder(GLfloat radius, GLfloat height, float R,float G, float B, bool flagff=false)
{
    GLfloat x              = 0.0;
    GLfloat y              = 0.0;
    GLfloat anglepp         = 0.0;
    GLfloat angle_stepsizepp = 0.1;

    /** Draw the tube */
    if(flagff==false){
      glColor3f(R,G,B);
      glBegin(GL_QUAD_STRIP);
      angle = 0.0;
          while( anglepp < 2*PI ) {
              x = radius * cos(anglepp);
              y = radius * sin(anglepp);
              glVertex3f(x, height, y  );
              glVertex3f(x, 0.0, y);
              anglepp = anglepp + angle_stepsizepp;
          }
          glVertex3f(radius, height, 0.0);
          glVertex3f(radius, 0.0, 0.0);
      glEnd();

      /** Draw the circle on top of cylinder */
      glColor3f(R,G,B);
      glBegin(GL_POLYGON);
      angle = 0.0;
          while( anglepp < 2*PI ) {
              x = radius * cos(anglepp);
              y = radius * sin(anglepp);
              glVertex3f(x, height, y);
              anglepp = anglepp + angle_stepsizepp;
          }
          glVertex3f(radius, height, y);
    }
    else
    {
      glColor3f(R,G,B);
      glBegin(GL_QUAD_STRIP);
      angle = 0.0;
          while( anglepp < 2*PI ) {
              x = radius * cos(anglepp);
              y = radius * sin(anglepp);
              glVertex3f(height, y, x );
              glVertex3f(0.0, y, x);
              anglepp = anglepp + angle_stepsizepp;
          }
          glVertex3f(height, 0.0, radius);
          glVertex3f(0.0, 0.0, radius);
      glEnd();

      /** Draw the circle on top of cylinder */
      glColor3f(R,G,B);
      glBegin(GL_POLYGON);
      angle = 0.0;
          while( anglepp < 2*PI ) {
              x = radius * cos(anglepp);
              y = radius * sin(anglepp);
              glVertex3f(height, y, x);
              anglepp = anglepp + angle_stepsizepp;
          }
          glVertex3f(height, y, radius);
    }
    glEnd();
}

void drawVehicle(float r, float g, float b, float leng=1.0, float wid=1.0, float ht=1.0) {

  glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
glBegin(GL_QUADS);        // Draw The Cube Using quads
    glColor3f(r,b,g);    // Color Blue
    glVertex3f( leng/2.0, ht,0);    // Top Right Of The Quad (Top)
    glVertex3f(0.0, ht,0);    // Top Left Of The Quad (Top)
    glVertex3f(0, ht, wid);    // Bottom Left Of The Quad (Top)
    glVertex3f( leng/2.0, ht, wid);    // Bottom Right Of The Quad (Top)
    glColor3f(r,b,g);      // Color Orange
    glVertex3f( leng/2.0,0.0f, wid);    // Top Right Of The Quad (Bottom)
    glVertex3f(-leng/2.0,0, wid);    // Top Left Of The Quad (Bottom)
    glVertex3f(-leng/2.0,0,0);    // Bottom Left Of The Quad (Bottom)
    glVertex3f( leng/2.0,0,0);    // Bottom Right Of The Quad (Bottom)
    glColor3f(r,b,g);      // Color Red
    glVertex3f( leng/2.0, ht/2, wid);    // Top Right Of The Quad (Front)
    glVertex3f(-leng/2.0, ht/2, wid);    // Top Left Of The Quad (Front)
    glVertex3f(-leng/2.0,0, wid);    // Bottom Left Of The Quad (Front)
    glVertex3f( leng/2.0,0, wid);    // Bottom Right Of The Quad (Front)
    glColor3f(r,b,g);      // Color Yellow
    glVertex3f( leng/2.0,0.0f,0);    // Top Right Of The Quad (Back)
    glVertex3f(-leng/2.0,0.0f,0);    // Top Left Of The Quad (Back)
    glVertex3f(-leng/2.0, ht,0);    // Bottom Left Of The Quad (Back)
    glVertex3f( leng/2.0, ht,0);    // Bottom Right Of The Quad (Back)
    glColor3f(r,b,g);      // Color Blue
    glVertex3f(0, ht, wid);    // Top Right Of The Quad (Left)
    glVertex3f(-leng/2.0, ht,0);    // Top Left Of The Quad (Left)
    glVertex3f(-leng/2.0,0,0);    // Bottom Left Of The Quad (Left)
    glVertex3f(0,0, wid);    // Bottom Right Of The Quad (Left)
    glColor3f(r,b,g);      // Color Violet
    glVertex3f( leng/2.0, ht,0);    // Top Right Of The Quad (Right)
    glVertex3f( 0, ht, wid);    // Top Left Of The Quad (Right)
    glVertex3f( 0,0.0f, wid);    // Bottom Left Of The Quad (Right)
    glVertex3f( leng/2.0,0.0f,0);    // Bottom Right Of The Quad (Right)
  glEnd();



}
