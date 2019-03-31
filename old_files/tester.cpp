

//#include <GL/glut.h> for linux
#include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h for mac
//#include <GLUT/glew.h>
#include <stdlib.h>
#include <GLFW/glfw3.h>
#include <math.h>
#include <iostream>
//include <glad/glad.h> 
#define GL_SILENCE_DEPRECATION
//#include <GLUT/glm.hpp>
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
int CurrentWidth = 640,
    CurrentHeight = 640;
float **graphic_road;
char **road; // we need to make this the char array which takes in input from the simulator
float angle = 0.0f;
float red=1.0f;
float green=1.0f;
float blue =1.0f;
float xc=15.0, zc=15.0;
// actual vector representing the camera's direction
float lx=0.0f,lz=-1.0f;
// XZ position of the camera
float x=0.0f,z=0.0f;
float y=18.0f;
int plot_convert(char **road, int width, int length)
{
   int size=0;
   for(int i=0;i<length;++i)
   {
      for(int j=0;j<width;++j)
      {
         if (road[i][j]=='b'|road[i][j]=='c')
         {
            size++;

         }
         
      }
   }
   //int size=(length)*(width)/2;
   graphic_road=new float*[size+1];
   for(int i=0;i<size;i++)
   {
      graphic_road[i]=new float[2];
   }
   int cur_leng=0;
   for(int i=0;i<length;++i)
   {
      for(int j=0;j<width;++j)
      {
         if (road[i][j]=='b'|road[i][j]=='c')
         {
            graphic_road[cur_leng][0]=-(float)i-1;
            graphic_road[cur_leng][1]=-(float)j-2;
            cur_leng++;

         }
         
      }
   }
  

   return cur_leng;

}
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

void drawSnowMan() {

  glColor3f(1.0f, 1.0f, 1.0f);

// Draw Body
  glBegin(GL_QUADS);        // Draw The Cube Using quads
    glColor3f(0.0f,1.0f,0.0f);    // Color Blue
    glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Top)
    glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Top)
    glVertex3f(-1.0f, 1.0f, 1.0f);    // Bottom Left Of The Quad (Top)
    glVertex3f( 1.0f, 1.0f, 1.0f);    // Bottom Right Of The Quad (Top)
    glColor3f(1.0f,0.5f,0.0f);    // Color Orange
    glVertex3f( 1.0f,-1.0f, 1.0f);    // Top Right Of The Quad (Bottom)
    glVertex3f(-1.0f,-1.0f, 1.0f);    // Top Left Of The Quad (Bottom)
    glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Bottom)
    glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Bottom)
    glColor3f(1.0f,0.0f,0.0f);    // Color Red    
    glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Front)
    glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Front)
    glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Front)
    glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Front)
    glColor3f(1.0f,1.0f,0.0f);    // Color Yellow
    glVertex3f( 1.0f,-1.0f,-1.0f);    // Top Right Of The Quad (Back)
    glVertex3f(-1.0f,-1.0f,-1.0f);    // Top Left Of The Quad (Back)
    glVertex3f(-1.0f, 1.0f,-1.0f);    // Bottom Left Of The Quad (Back)
    glVertex3f( 1.0f, 1.0f,-1.0f);    // Bottom Right Of The Quad (Back)
    glColor3f(0.0f,0.0f,1.0f);    // Color Blue
    glVertex3f(-1.0f, 1.0f, 1.0f);    // Top Right Of The Quad (Left)
    glVertex3f(-1.0f, 1.0f,-1.0f);    // Top Left Of The Quad (Left)
    glVertex3f(-1.0f,-1.0f,-1.0f);    // Bottom Left Of The Quad (Left)
    glVertex3f(-1.0f,-1.0f, 1.0f);    // Bottom Right Of The Quad (Left)
    glColor3f(1.0f,0.0f,1.0f);    // Color Violet
    glVertex3f( 1.0f, 1.0f,-1.0f);    // Top Right Of The Quad (Right)
    glVertex3f( 1.0f, 1.0f, 1.0f);    // Top Left Of The Quad (Right)
    glVertex3f( 1.0f,-1.0f, 1.0f);    // Bottom Left Of The Quad (Right)
    glVertex3f( 1.0f,-1.0f,-1.0f);    // Bottom Right Of The Quad (Right)
  glEnd();         
}
//float angle = 0.0f;

void renderScene(void) {

   // Clear Color and Depth Buffers

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Reset transformations
   glLoadIdentity();
   // Set the camera
  gluLookAt(   xc+x, y, zc+z,
         -100.0f+10*lx, 0.0f,  -100.0f+10*lz,
         0.0f, 3.0f,  0.0f);


        // Draw ground
   glColor3f(0.9f, 0.9f, 0.9f);
   glBegin(GL_QUADS);
      glVertex3f(-100.0f, 0.0f, -100.0f);
      glVertex3f(-100.0f, 0.0f,  100.0f);
      glVertex3f( 100.0f, 0.0f,  100.0f);
      glVertex3f( 100.0f, 0.0f, -100.0f);
   glEnd();

        // Draw 36 SnowMen
   

   int w=10;
   
   int l=3;
   road=new char*[l];
   for(int i=0;i<l;++i)
   {
      road[i]=new char[w];
      road[i]="b--g--c---";
   }
   road[2]="---c------";

   int n;
   n=plot_convert(road,w,l);
   //graphic_road=new float*[5];
  /*for(int i=0;i<5;i++)
   {
      graphic_road[i]=new float[2];
      graphic_road[i][0]=-(i+0.5);
      graphic_road[i][1]=-(i+1.0);
   }*/
   //*n=5;
   //for(int i=0;i<*n;++i)
   //{
   //std::cout<<"WHYYYYYY"<<std::endl;
   //std::cout<<n<<std::endl;
   if(n!=0)
      //std::cout<<graphic_road[0][0]<<"\t";
   //}
   for(int i=0; i<3;++i)
   {
      glPushMatrix();
      glTranslatef((graphic_road[i][0]-i)*10.0,0,(graphic_road[i][1]-i) * 15.0);
      drawSnowMan();
      glPopMatrix();
   }

   /*for(int i = -5; i < 1; i++)
      for(int j=-5; j < 1; j++) {
         glPushMatrix();
         glTranslatef(i*10.0,0,j * 10.0);
         drawSnowMan();
         glPopMatrix();
      }*/

   glutSwapBuffers();
}

void processSpecialKeys(int key, int xx, int yy) {

   float fraction = 0.1f;

   switch (key) {
      case GLUT_KEY_LEFT :
         angle -= 0.01f;
         lx = sin(angle);
         lz = -cos(angle);
         break;
      case GLUT_KEY_RIGHT :
         angle += 0.01f;
         lx = sin(angle);
         lz = -cos(angle);
         break;
      case GLUT_KEY_UP :
         x += lx * fraction;
         z += lz * fraction;
         break;
      case GLUT_KEY_DOWN :
         x -= lx * fraction;
         z -= lz * fraction;
         break;
      case GLUT_KEY_F1:
         y=y+0.05;

         break;
      case GLUT_KEY_F2:
         y=y-0.05;
         
         break;
   }
}
void processNormalKeys(unsigned char key, int x, int y) {

   if (key == 'q')
      exit(0);
}



int main(int argc, char **argv) {

   // init GLUT and create window
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(800,800);
   glutCreateWindow("Lighthouse3D- GLUT Tutorial");
   //std::cout<<"HERERERERE"<<std::endl;
   // register callbacks
   glutDisplayFunc(renderScene);
   glutReshapeFunc(changeSize);
   glutIdleFunc(renderScene);
   glutKeyboardFunc(processNormalKeys);
   glutSpecialFunc(processSpecialKeys);

   // OpenGL init
   glEnable(GL_DEPTH_TEST);

   // enter GLUT event processing cycle
   glutMainLoop();

   // enter GLUT event processing cycle
  // glutMainLoop();

   return 1;
}