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
#include "common.h"
#include <thread>
using namespace std;
std::random_device random_gen;
std::mt19937 gen(random_gen());
std::uniform_real_distribution<>dis(0.0,1.0);

  bool flag=false;
  bool signal1=false;
  int present_time;
  int vehicle_id;
  int default_max_xspeed=3;
  int default_max_acceleration = 1;
  int default_lanechange_vertical_speed = 1;
  int default_lanechange_horizontal_speed = 1;
  int signal_pos = 8;
  int default_xpos = 0;
  int default_ypos = 0;
  int default_vertical_velocity  = 0;
  int default_horizontal_velocity = 0;
  int road_len = 30;
  int road_wid = 5;
  char symbol_name='\0';
  float default_lane_change_freq = 0.4;



float getRandom(){
  //cout<<&dis<<endl; to check the address is same or not
  return (float) dis(gen);
};//returns a float between 0 to 1 in uniform distribution
