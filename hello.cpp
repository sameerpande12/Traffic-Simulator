

//#include <GL/glut.h>
#include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h
//#include <GLUT/glew.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
//include <glad/glad.h> 
#define GL_SILENCE_DEPRECATION
//#include <GLUT/glm.hpp>
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
int CurrentWidth = 640,
    CurrentHeight = 640;
float angle = 0.0f;
float red=1.0f;
float green=1.0f;
float blue =1.0f;
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
   gluPerspective(30.0f, ratio, 1.0f, 100.0f);

   // Get Back to the Modelview
   glMatrixMode(GL_MODELVIEW);
}

//float angle = 0.0f;

void renderScene(void) {

   // Clear Color and Depth Buffers
   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Reset transformations
   glLoadIdentity();
   // Set the camera
   gluLookAt(  0.0f, 0.0f, 10.0f,
            0.0f, 0.0f,  0.0f,
            0.0f, 1.0f,  0.0f);

   glRotatef(angle, 0.0f, 1.0f, 0.0f);
   glColor3f(red,green,blue);
   glBegin(GL_TRIANGLES);
      glVertex3f( 0.0f, 1.0f, 0.0f);
      glVertex3f( -1.50f, 1.0f, 0.0);
      glVertex3f( 0.0f, -1.5f, 0.0);
   glEnd();

   angle+=0.5f;

   glutSwapBuffers();
}
void processNormalKeys(unsigned char key, int x, int y) {

   if (key == 'q')
      exit(0);
}

void processSpecialKeys(int key, int x, int y) {

   switch(key) {
      case GLUT_KEY_LEFT :
            red = 1.0;
            green = 0.0;
            blue = 0.0; break;
      case GLUT_KEY_UP :
            red = 0.0;
            green = 1.0;
            blue = 0.0; break;
      case GLUT_KEY_RIGHT :
            red = 0.0;
            green = 0.0;
            blue = 1.0; break;
   }
}

int main(int argc, char **argv) {

   // init GLUT and create window
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(800,800);
   glutCreateWindow("Lighthouse3D- GLUT Tutorial");

   // register callbacks
   glutDisplayFunc(renderScene);
   glutReshapeFunc(changeSize);
   glutIdleFunc(renderScene);
   glutKeyboardFunc(processNormalKeys);
   glutSpecialFunc(processSpecialKeys);

   // enter GLUT event processing cycle
   glutMainLoop();

   return 1;
}