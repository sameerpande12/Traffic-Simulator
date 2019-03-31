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
using namespace std;
/*
extern bool flag=false;
extern bool signal1=false;
extern int present_time;
extern int vehicle_id;
extern int default_max_xspeed=3;
extern int default_max_acceleration = 1;
extern int default_lanechange_vertical_speed = 1;
extern int default_lanechange_horizontal_speed = 1;
extern int signal_pos = 8;
extern int default_xpos = 0;
extern int default_ypos = 0;
extern int default_vertical_velocity  = 0;
extern int default_horizontal_velocity = 0;
extern int road_len = 30;
extern int road_wid = 5;
extern char symbol_name='\0';
extern float default_lane_change_freq = 0.4;
*/

extern bool flag;
extern bool signal1;
extern int present_time;
extern int vehicle_id;
extern int default_max_xspeed;
extern int default_max_acceleration ;
extern int default_lanechange_vertical_speed;
extern int default_lanechange_horizontal_speed ;
extern int signal_pos ;
extern int default_xpos ;
extern int default_ypos ;
extern int default_vertical_velocity  ;
extern int default_horizontal_velocity ;
extern int road_len ;
extern int road_wid ;
extern char symbol_name;
extern float default_lane_change_freq ;


float getRandom();
