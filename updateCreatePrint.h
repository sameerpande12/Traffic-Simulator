#pragma once
#include "common.h"
#include "Road.h"
#include "template.h"
#include "Vehicle.h"

#include <stdlib.h>
#include <math.h>
#include <iostream>

using namespace std;
bool compareChar(char &c1, char &c2);
bool strcmping(std::string &str1, std::string &str2);

void printRaod(Road* rd);
void updateRoad(Road* rd);
void updatePositionsOnRoad(Road*rd, bool print = false);
void updateRoad(Road* road, int t, bool print);
Vehicle* createVehicleOnRoad( string type, string color , int length, int width, Road* rd, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq);
void addVehicleOnRoad(Vehicle* newVehicle , Road * rd);
Vehicle* createVehicle( string type, string color , int length, int width, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq);
template_vehicle find_in(string name, vector<template_vehicle> vec);
