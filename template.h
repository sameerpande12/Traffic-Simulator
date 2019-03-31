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
#include "common.h"
using namespace std;

class template_road
{
public:
  int id, length,width,signal;
}
;
class template_vehicle{
public:
  string type; string color ; int length; int width;  char symbol; int lane_no; int column_no; int max_speed; int max_acceleration; int lanechange_vertical_speed; int lanechange_horizontal_speed;
  float lane_change_freq;
  template_vehicle();

}
;
