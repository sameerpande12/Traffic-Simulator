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
#include "template.h"
using namespace std;

template_vehicle::template_vehicle(){
  type="";
  length=2;
  width=2;
  symbol=' ';
  max_speed=default_max_xspeed;
  max_acceleration=default_max_acceleration;
  lanechange_vertical_speed=default_lanechange_vertical_speed;
  lanechange_horizontal_speed=default_lanechange_horizontal_speed;
}
