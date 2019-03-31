#pragma once
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



void changeSize(int w, int h);

void draw_cylinder(GLfloat radius, GLfloat height, float R,float G, float B, bool flagff=false);

void drawVehicle(float r, float g, float b, float leng=1.0, float wid=1.0, float ht=1.0);
