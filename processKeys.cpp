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
#include "common.h"

#include <GL/glut.h> //for linux
//#include <GLUT/glut.h>  // GLUT, includes glu.h and gl.h for mac
//#include <GLUT/glew.h>
//#include <GLFW/glfw3.h>
//include <glad/glad.h>


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
      case GLUT_KEY_F3:
         perspective_angle=perspective_angle+1.0;

         break;
      case GLUT_KEY_F4:
         perspective_angle=perspective_angle-1.0;

         break;
      case GLUT_KEY_F5:
         blooky=blooky-0.1;

         break;
      case GLUT_KEY_F6:
         blooky=blooky+0.1;

         break;
      case GLUT_KEY_F7:
         blooky=blooky-0.1;

         break;
      case GLUT_KEY_F8:
         blooky=blooky+0.1;

         break;
   }

}
void processNormalKeys(unsigned char key, int x, int y) {

   if (key == 'q')
      exit(0);
    else if(key =='a')
      perspective_angle=perspective_angle+1.0;
    else if(key== 'd')
      perspective_angle=perspective_angle-1.0;

}
