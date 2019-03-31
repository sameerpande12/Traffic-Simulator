
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
using namespace std;

class Road;
class Vehicle
{
public:

 //existential parameters
  int id;//ids should be unique for all the vehicles
  string type;
  string color;
  int length;
  int width;
  char symbol;
  Road* on_road;//road on which vehicle is moving

  //movement parameters
  int max_xspeed = 3;
  int max_acceleration = 1;
  float lane_change_freq = 0.5;
  int lanechange_horizontal_speed = 1;
  int lanechange_vertical_speed = 1;
  //overtaking done only from right side of vehicle to be overtaken
  //bool changing_lane = false;


  /*probability of overtaking given overtaking is possible*/
  /*we define overtaking is possible when a smaller vehicle can shift to a lane(s) on which no trace of the vehicle to be overtaken is found*/
  /*for time being assume instantaneous breaking is possible:- Scenario is almost true for indian drivers xD*/

  int velocity[2]={0,0};//velocity[0] is x_velocity and velocity[1] is y_velocity
  //int lane_no;//represents lane on which top right corner of vehicle is

  int pos[2] = {0,0};
  void changeVelocity();
  void changePosition();

}
