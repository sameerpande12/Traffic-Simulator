/*
Axes:- Right -> Positive,
       Down -> Positive

references- https://www.geeksforgeeks.org/stdany-class-in-c/
*/
#include<any>
#include<cstring>
#include<string>
#include <iostream>

using namespace std;

class Road{
  int id;//
  int length;//
  int width;// make them of type final
  int** road_matrix;
}

class Vehicle{
public:

 //existential parameters
  int id;
  string type;
  string color;
  int length;
  int width;


  //movement parameters
  int max_xspeed = 3;
  int max_acceleration = 1;
  float lane_change_freq = 0.3;
  float overtake_freq = 0.3;
  int overtake_horizontal_speed = 1;
  //overtaking done only from right side of vechilce to be overtaken
  bool changing_lane = false;
  /*probability of overtaking given overtaking is possible*/
  /*we define overtaking is possible when a smaller vechile can shift to a lane(s) on which no trace of the vehicle to be overtaken is found*/
  /*for time being assume instantaneous breaking is possible:- Scenario is almost true for indian drivers xD*/

  int velocity[2]={0,0};//velocity[0] is x_velocity and velocity[1] is y_velocity
  int lane_no;//represents lane on which top right corner of vechile is
  Road on_road;//road on which vehicle is moving
  int pos[2] = {1,1};//initial position (x,y) coordinates


  void changeVelocity(){



  }

  void changePosition(){



  }


}

int main(char** argc, int argv){


}
