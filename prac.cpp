/*
Axes:- Right -> Positive,
       Down -> Positive

(i,j) for roads-> indexing pattern
----------------------
0,0     0,1       0,2
1,0     1,1       1,2
-----------------------
references- https://www.geeksforgeeks.org/stdany-class-in-c/
https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
*/
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



#include "processKeys.h"
#include "common.h"
#include "Road.h"
#include "template.h"
#include "Vehicle.h"
#include "updateCreatePrint.h"


#define PI 3.14159
#define GL_SILENCE_DEPRECATION

using namespace std;

int no_arguments=0;
char **names;
Road road;
int proc(int argc, char** argv)
{
  present_time = 1;//initiating the time
  vehicle_id = 1;// initiating vehicle_id: each vehicle has its own id

  road.length = road_len;
  road.width = road_wid;
  road.id = 1;
  int vehicle_no=0;
  road.signal_pos = signal_pos;
//  std::vector< Vehicle> vec_vehicle;

  signal1=true;
  if(argc<2)
  {
    cout<<"Error: no config file passed"<<endl;
    return -1;
  }
  ifstream file(argv[1]);
  std::stringstream buffer;
  if ( file )
  {
        buffer << file.rdbuf();
        file.close();
  }

        std::vector<std::string> result;
        //std::stringstream stream(string);
        std::string word;
        while (std::getline(buffer, word, '\n'))
        {
            size_t found = word.find("#");
            if(word.length()==0)
              {
                result.push_back(word);
                continue;
              }
            if (found != string::npos)
                word=word.substr(0,found);
            if(word.length()>0)
              result.push_back(word);
        }
      result.push_back("");
     auto k=result.begin();
     for(;k!=result.end();k++)
     {
      //cout<<*k<<endl;
     }
  std::vector< std::vector<std::string> > tokens;  //every element of vector stores a vector storing one section line by line
  auto it=result.begin();
  std::vector<std::string> temp;
  for(;it!=result.end();it++)
  {

      if(it->length()==0)
        {if(temp.size()>0)
          {tokens.push_back(temp);}
        temp.clear();}
      else
        temp.push_back(*it);
  }
  /*for(auto it1=tokens.begin();it1!=tokens.end();it1++)
  {
    for(auto it2=it1->begin();it2!=it1->end();it2++)
    {
      std::cout<<*it2<<std::endl;
    }
    std::cout<<endl<<"--------------------------------------------------"<<std::endl;
  }*/
  std::vector< template_vehicle > all_vehicles;
  bool mode=false;
  for(auto it1=tokens.begin();it1!=tokens.end();it1++)
  {
    //std::vector<std::string> instr=*it;
    int i=0;
    std::string type, ins=*(it1->begin());
    if(ins.length()==0)
      continue;
    if(ins.substr(0,5).compare("START")==0)
      {
        mode=true;
        continue;
      }
    if(mode==false)
     {
      size_t findc=ins.find("_");
      if (findc != string::npos)
        {
          type=ins.substr(0,findc);
        }
      else
      {
        cout<<"Error while parsing, no underscore used "<<endl;
        return -1;
      }
      if(type.compare("Vehicle")==0)
        {
          template_vehicle new_vehic;
          try{
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Vehicle_Type")==0)
            {
                new_vehic.type=value;
            }
            else if(property.compare("Vehicle_Length")==0)
            {
                new_vehic.length=stoi(value);
            }
            else if(property.compare("Vehicle_Width")==0)
            {
                new_vehic.width=stoi(value);
            }
            else if(property.compare("Vehicle_MaxSpeed")==0)
            {
                new_vehic.max_speed=stoi(value);
            }
            else if(property.compare("Vehicle_MaxAcceleration")==0)
            {
                new_vehic.max_acceleration=stoi(value);
            }
            else if(property.compare("Vehicle_LChange_v_speed")==0)
            {
                new_vehic.lanechange_vertical_speed=stoi(value);
            }
            else if(property.compare("Vehicle_LChange_h_speed")==0)
            {
                new_vehic.lanechange_horizontal_speed=stoi(value);
            }


          }
        }
        catch(std::invalid_argument& e){
          cout<<"error in conversion of vehicle"<<endl;}
          all_vehicles.push_back(new_vehic);
        }
      else if(type.compare("Default")==0)
        {
          try{template_vehicle new_vehic;
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Default_MaxSpeed")==0)
            {
                default_max_xspeed=stoi(value);
            }
            else if(property.compare("Default_Acceleration")==0)
            {
                default_max_acceleration=stoi(value);
            }
            else if(property.compare("Default_LChange_v_speed")==0)
            {
                default_lanechange_vertical_speed=stoi(value);
            }
            else if(property.compare("Default_LChange_h_speed")==0)
            {
                default_lanechange_horizontal_speed=stoi(value);
            }
          }
         }
         catch(std::invalid_argument& e){
          cout<<"error in conversion of default"<<endl;}
         }
        else if(type.compare("Road")==0)
        {
          try{
          for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::string property, grabage, value;
            istringstream ss(*it2);
            ss>>property;
            ss>>grabage;
            ss>>value;
            if(property.compare("Road_Length")==0)
            {
                road.length=stoi(value);
                road_len=road.length;
            }
            else if(property.compare("Road_Width")==0)
            {
                road.width=stoi(value);
                road_wid=road.width;
            }
            else if(property.compare("Road_Id")==0)
            {
              road.id=stoi(value);
            }
            else if(property.compare("Road_Signal")==0)
            {
              road.signal_pos=stoi(value);
            }
            *(road.signal_color)='R';
          }
            char** road_matrix = new char*[road_wid];
            for(int i = 0;i<road_wid;i++){
              road_matrix[i]=new char[road_len];
             for(int j = 0;j<road_len;j++)road_matrix[i][j]=' ';
            }
            road.road_matrix = road_matrix;
        }
         catch(std::invalid_argument& e){
          cout<<"error in conversion of road"<<endl;}
        }
      }
    else
    {

      string name, color="red";
      //cout<<"after start"<<endl;
      int length=2, width=1, lane_no=0, col_no=0, max_speed=default_max_xspeed, max_acceleration=default_max_acceleration, lnchangeh=default_lanechange_horizontal_speed, lnchangev=default_lanechange_vertical_speed, v_vel=0, h_vel=0;float lchang_f=0.06;
      for(auto it2=it1->begin();it2!=it1->end();it2++)
          {
            std::vector<std::string> attributes;
            //cout<<"Input: "<<*it2<<endl;
            istringstream ss(*it2);
            while(ss)
            {
              string temp;
              ss>>temp;

              if(temp.compare(" ")!=0)
              {

                attributes.push_back(temp);
              }
            }
            //cout<<"Abnormal behavious "<<endl;
            if(attributes[0].compare("Pass")==0)
            {
              try{updateRoad(&road,stoi(attributes[1]),true);}
              catch(std::invalid_argument& e){
              cout<<"error in conversion of time in PASS"<<endl;}

              continue;
            }
            //cout<<"Abnormal behavious sads"<<endl;
            if(attributes[0].compare("Signal")==0)
            {

              *(road.signal_color)=toupper(attributes[1].at(0));

              continue;
            }
            if(attributes[0].compare("END")==0)
            {
              if(*(road.signal_color)=='G')
              {
                while(!road.vehicles.empty())
                {
                  updateRoad(&road,1,false);
                }
              }
              else
                continue;
            }
            //cout<<"Abnormal behavious dfd"<<endl;
            template_vehicle to_load=find_in(attributes[0], all_vehicles);
            if(to_load.type.length()==0)      //no vehicle found
              {
                cout<<attributes[0]<<" was not defined"<<endl;
                continue;
              }
            else
            {
              name=to_load.type;
              length=to_load.length;
              width=to_load.width;
              max_speed=to_load.max_speed;
              max_acceleration=to_load.max_acceleration;
              lnchangeh=to_load.lanechange_horizontal_speed;
              lnchangev=to_load.lanechange_vertical_speed;
              color=attributes[1];
              //cout<<"Abnormal behavious before error"<<endl;
              //cout<<"attributes size is "<<attributes.size()<<endl;
              try{
              if(attributes.size()>=4)
               {
                lane_no=stoi(attributes[2]);
                if(attributes.size()>=5)
                      {
                        col_no=stoi(attributes[3]);
                        if(attributes.size()>=6)
                        {
                          v_vel=stoi(attributes[4]);
                          if(attributes.size()>=7)
                          {
                            h_vel=stoi(attributes[5]);
                            if(attributes.size()>=8)
                              lchang_f=stof(attributes[6]);

                          }
                        }
                      }
                }
                }
                catch(std::invalid_argument& e){
              cout<<"error in conversion of non existent "<<endl; return -1;}
              catch(std::out_of_range& e){
              cout<<"error in conversion of something not present "<<endl; return -1;}
              //cout<<"ewhy stop here?cs "<<endl;
              auto iter = road.vehicles.begin();
            while(flag==false)
            {
                 std::this_thread::sleep_for(std::chrono::milliseconds(100));

            }
             //cout<<"ewhy stop here? "<<endl;
            Vehicle *temp_vehicle=((createVehicle(name,color,length,width,symbol_name++,lane_no,col_no,max_speed,max_acceleration,lnchangev,lnchangeh,v_vel, h_vel, lchang_f)));
            while(true) //incomplete check for clash but it works assuming contagious vehicles
            {
             updateRoad(&road,1,true);
              bool isEmpty = true;
               for(int i = lane_no;i<lane_no+width;i++){
                for(int j =col_no;j<col_no+length;j++){
                  if( i < road.width && j<road.length){
                    if(road.road_matrix[i][j]!=' '){
                      isEmpty = false;
                      break;
                    }
                  }
                }
                if(isEmpty == false)break;
              }
             if(isEmpty)break;
             else updateRoad(&road,1,true);
            }
            addVehicleOnRoad(temp_vehicle, &road);
            if(symbol_name==' ')
              symbol_name++;
            vehicle_no++;
           // cout<<"why"<<endl;
            //printRoad(&road);
            updatePositionsOnRoad(&road,false);
            //cout<<"why1.5"<<endl;
            updateRoad(&road,1,true);
            //cout<<"why2"<<endl;
            }

          }

    }

  }



  return 0;
}

void *main_calls(void *a)
{
  //cout<<"here???? in main calls"<<endl;
  if(proc(no_arguments, names)==-1)
    {
      cout<<"error occured "<<endl;
      exit(1);
    }
  void *aaa;
  return aaa;
}



//#include <GLUT/glm.hpp>
/* Handler for window-repaint event. Call back when the window first appears and
   whenever the window needs to be re-painted. */
int CurrentWidth = 640,
    CurrentHeight = 640;
float **graphic_road;
char **road_; // we need to make this the char array which takes in input from the simulator
float angle = 0.0f;
float red=1.0f;
float green=1.0f;
float blue =1.0f;
float xc=15.0, zc=15.0;
float perspective_angle=45.0f;
// actual vector representing the camera's direction
float lx=-0.813357,lz=-0.581764f;
// XZ position of the camera
float x=-1.88822f,z=2.6549f;
float y=10.0f;
float blookx=0.0;
float blooky=0.0;
std::vector<Vehicle*> lanes;
int plot_convert(char **roadi, int width, int length)
{
   int size=0;
   lanes.clear();
   for(int i=0;i<width;++i)
   {
      for(int j=0;j<length;++j)
      {
         if (roadi[i][j]!=' ')
         {


            try{
              Vehicle *graph_vehicle=road.symbol_maps.at(roadi[i][j]);
            lanes.push_back((graph_vehicle));}
               catch(std::out_of_range& e){
              cout<<"error in conversion of non plot_convert "<<roadi[i][j]<<endl; return -1;}



         }

      }
   }
   return size;

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
//float angle = 0.0f;

void renderScene(void) {

   // Clear Color and Depth Buffers

   glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   // Reset transformations
   glLoadIdentity();
   // Set the camera
  gluLookAt(   x-road_len/1.75+blookx, y, z-road_len/2.25+blooky,
         x-1.0+lx, 0.0f,  z+1.0+lz,
         0.0f, 10.0f,  0.0f);


        // Draw ground
   glColor3f(0.6f, 0.7f, 0.7f);
   glBegin(GL_QUADS);
      glVertex3f(-road_len/2.0, -0.3f, -road_wid/2.0-1.0);
      glVertex3f(-road_len/2.0, -0.3f,  road_wid/2.0+3.0);
      glVertex3f( road_len/2.0, -0.3f,  road_wid/2.0+3.0);
      glVertex3f( road_len/2.0, -0.3f, -road_wid/2.0-1.0);
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3f(-road_len/2.0, -0.4f, -road_wid/2.0);
      glVertex3f(-road_len/2.0, -0.4f,  -road_wid/2.0-6.5);
      glVertex3f( road_len/2.0, -0.4f,  -road_wid/2.0-6.5);
      glVertex3f( road_len/2.0, -0.4f, -road_wid/2.0);
      glColor3f(0.0f, 0.0f, 0.0f);
      glVertex3f(-road_len/2.0, -0.4f, road_wid/2.0+8.5);
      glVertex3f(-road_len/2.0, -0.4f,  road_wid/2.0);
      glVertex3f( road_len/2.0, -0.4f,  road_wid/2.0);
      glVertex3f( road_len/2.0, -0.4f, -road_wid/2.0+8.5);
   glEnd();




   int w=road_wid;

   int l=road_len;
         /*road_=new char*[l];
         for(int i=0;i<l;++i)
         {
            road_[i]=new char[w];
            road_[i]="b--g--c---";
         }
         road_[2]="---c------";*/

   int n;
   road_=road.road_matrix;
   /*while( l!=30)
   {
    cout<<"road.length is "<<road.length<<endl;
    cout<<"waiting"<<endl;
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   }*/
   //cout<<"before plot convert"<<endl;
   //while(road.length<l)
   flag=true;
   while(signal1==false)std::this_thread::sleep_for(std::chrono::milliseconds(1000));

   n=plot_convert(road_,w,l);

   glPushMatrix();
   glTranslatef(road_len/2.0-road.signal_pos,0,-road_wid/2.0-3.0);
   draw_cylinder(0.2,8.0,0.4,0.4,0.4);

    glPopMatrix();
    if(*(road.signal_color)=='R'){
      glColor3f(1,0,0);
      GLUquadric *quad;
      quad = gluNewQuadric();
      glPushMatrix();
      glTranslatef(road_len/2.0-road.signal_pos-1.0,6.5,-road_wid/2.0-3.3);
      glRotatef(90.0, 0.0f, 1.0f, 0.0f);
      gluDisk(quad,0.0,0.6,100.0,100.0);
      glPopMatrix();
    }
    else
    {
      glColor3f(0,1,0);
      GLUquadric *quad;
      quad = gluNewQuadric();
      glPushMatrix();
      glTranslatef(road_len/2.0-road.signal_pos-1.0,7.5,-road_wid/2.0-3.3);
      glRotatef(90.0, 0.0f, 1.0f, 0.0f);
      gluDisk(quad,0.0,0.6,100.0,100.0);
      glPopMatrix();
    }
   //drawVehicle(r,b,g,temp.length,temp.width,veh_height);
      //cout<<"before pop"<<endl;

   //cout<<"after plot convert "<<endl;
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
   //cout<<"why here mate "<<endl;
   //x
   auto it=lanes.begin();
   float r=0.0f, b=0.0f, g=0.0f;
   string red_clr="red";
   string green_clr="green";
   string blue_clr="blue";
   int i=0;
   for(;it!=lanes.end();it++)
   {
    Vehicle temp=**it;
    glPushMatrix();

    glTranslatef(-temp.pos[1]+road_len/2.0+0.5,0,-temp.pos[0]+2.5-temp.width+road_wid/2.0);
    if( strcmping(temp.color,red_clr))
      {
        r=1.0f;
        g=0.25f;
      }
    else if( strcmping(temp.color,green_clr))
      {
        g=0.8f;
      }
    else if( strcmping(temp.color,blue_clr))
      {
        b=1.0f;
        g=0.5f;
      }
    else
    {
      r=1.0;
      g=0.6;
    }
    float veh_height=2.0;
    string bustr="bus";
    string bikstr="bike";
    if( strcmping(temp.type, bustr))
      veh_height=3.5;
    else if( strcmping(temp.type, bikstr))
      veh_height=1.5;



    drawVehicle(r,b,g,temp.length/1.5,temp.width/1.5,veh_height);

    glPopMatrix();
    r=0.0;
    b=0.0;
    g=0.0;


   }

   glutSwapBuffers();
}



int main(int argc, char **argv) {

   // init GLUT and create window
   no_arguments=argc;
   names=argv;
   glutInit(&argc, argv);
   glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
   glutInitWindowPosition(100,100);
   glutInitWindowSize(1200,1200*(road_len/road_wid)/6);
   glutCreateWindow("Traffic Simulator");
   // register callbacks

   pthread_t threads;
   int rc;
   int i;

    rc = pthread_create(&threads, NULL, main_calls, (void *)i);

      if (rc) {
         cout << "Error:unable to create thread," << rc << endl;
         exit(-1);
      }

   std::this_thread::sleep_for(std::chrono::milliseconds(1000));
   // enter GLUT event processing cycle
   //flag=true;
   glutDisplayFunc(renderScene);
   glutReshapeFunc(changeSize);
   glutIdleFunc(renderScene);
   glutKeyboardFunc(processNormalKeys);
   glutSpecialFunc(processSpecialKeys);

   // OpenGL init
   glEnable(GL_DEPTH_TEST);
   glutMainLoop();

   // enter GLUT event processing cycle
  // glutMainLoop();

   return 1;
}
