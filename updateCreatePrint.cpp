
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
#include <stdlib.h>
#include <math.h>
#include <iostream>

#include "common.h"
#include "Road.h"
#include "template.h"
#include "Vehicle.h"


#define PI 3.14159
#define GL_SILENCE_DEPRECATION

using namespace std;

bool compareChar(char & c1, char & c2)
{
  if (c1 == c2)
    return true;
  else if (std::toupper(c1) == std::toupper(c2))
    return true;
  return false;
}

/*
 * Case Insensitive String Comparision
 */
bool strcmping(std::string & str1, std::string &str2)
{
  return ( (str1.size() == str2.size() ) &&
       std::equal(str1.begin(), str1.end(), str2.begin(), &compareChar) );
}


void updateRoad(Road* rd){//function to update road matrix

  map<int,Vehicle*>::iterator iter = rd->vehicles.begin();
  for(iter = rd->vehicles.begin();iter!= rd->vehicles.end();iter++){


    (iter->second)->changeVelocity();

  }



  std::vector<int> ids_to_remove;
  std::vector<char> syms_to_remove;
  for(iter = rd->vehicles.begin();iter!= rd->vehicles.end();iter++)(iter->second)->changePosition();
  iter = rd->vehicles.begin();


  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';


  for(iter = rd->vehicles.begin();iter!=rd->vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){

        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<rd->length){
          if(i+(iter->second)->pos[0]>=rd->width)continue;

          if( rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j] !=' '){
              iter->second->crashed = true;
              rd->symbol_maps[rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]]->crashed = true;
          }

          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;


        }


      }
    }
    if((iter->second)->pos[1]-(iter->second)->length+1>=rd->length){
         ids_to_remove.push_back(iter->first);
         syms_to_remove.push_back(iter->second->symbol);
    }
  }



  vector<int>::iterator vec_iter = ids_to_remove.begin();
  for(vec_iter = ids_to_remove.begin();vec_iter<ids_to_remove.end();vec_iter++){
    rd->vehicles.erase(*vec_iter);


  }
  vector<char>::iterator char_iter = syms_to_remove.begin();
  for(char_iter = syms_to_remove.begin();char_iter<syms_to_remove.end();char_iter++){
    rd->vehicles.erase(*char_iter);


  }

  //updates the road by one unit time
  present_time++;

}
void printRoad(Road* rd){
  cout<<"Signal Color:"<<*(rd->signal_color)<<endl;
  for(int i = 0;i<2*rd->length;i++)cout<<"-";
  cout<<endl;
  for(int i= 0;i<rd->width;i++){

    for(int j =0;j<rd->length;j++){
      if(rd->road_matrix[i][j]!=' ')
      {
        Vehicle *temp_vehc=rd->symbol_maps.at(rd->road_matrix[i][j]);
        cout<<temp_vehc->type.at(0);
      }
      else
        cout<<rd->road_matrix[i][j];
      if(j+1==rd->signal_pos)cout<<"|";else cout<<" ";
    }
    cout<<endl;
    for(int j =0;j<2*rd->length;j++)cout<<"-";
    cout<<endl;
  }
  cout<<endl;
}

void updatePositionsOnRoad(Road* rd,bool print=false){

  for(int i = 0;i<rd->width;i++)
   for(int j = 0;j<rd->length;j++)rd->road_matrix[i][j]=' ';

   map<int,Vehicle*> vehicles = rd->vehicles;
   map<int,Vehicle*>::iterator iter = vehicles.begin();

  for(iter = vehicles.begin();iter!=vehicles.end();iter++)
  {

    for(int i = 0;i<(iter->second)->width;i++){
      for(int j = 0;j<(iter->second)->length;j++){
        if((iter->second)->pos[1]>=j && (iter->second)->pos[1]-j<=rd->length){
          if(i+(iter->second)->pos[0]>=rd->width)continue;
          rd->road_matrix[i+(iter->second)->pos[0]][(iter->second)->pos[1]-j]=iter->second->symbol;
        }
      }
    }
  }

  if(print)printRoad(rd);
 // no change in time. Just to initialize the roadMatrix.
}



void updateRoad(Road* road, int t,bool print){
    if(t <=0 )cout<<"ERROR:Please enter valid time input"<<endl;
    else{

      for(int i=0;i<t;i++){

        updateRoad(road);



      if(print)printRoad(road);
      std::this_thread::sleep_for(std::chrono::milliseconds(1000));
      system("clear");
        cout<<endl;
      }

    }

}

Vehicle* createVehicleOnRoad( string type, string color , int length, int width, Road* rd, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq){
  Vehicle* newVehicle =  new Vehicle;
  newVehicle->id = vehicle_id;
  vehicle_id++;//update the vehicle id
  newVehicle->type = type;
  newVehicle->color = color;
  newVehicle->length = length;
  newVehicle->symbol = symbol;
  newVehicle->width = width;
  newVehicle->on_road = rd;
  newVehicle->pos[0] = lane_no;
  newVehicle->pos[1] = column_no;
  newVehicle->max_xspeed = max_speed;
  newVehicle->max_acceleration = max_acceleration;
  newVehicle->lanechange_vertical_speed = lanechange_vertical_speed;
  newVehicle->lanechange_horizontal_speed = lanechange_horizontal_speed;
  newVehicle->velocity[0] = ini_vertical_velocity;
  newVehicle->velocity[1] = ini_horizontal_velocity;
  newVehicle->lane_change_freq = lane_change_freq;
  (*rd).vehicles.insert(std::pair<int,Vehicle*>(newVehicle->id,newVehicle));

  (*rd).symbol_maps.insert(std::pair<char,Vehicle*>(newVehicle->symbol,newVehicle));
  return newVehicle;
}

void addVehicleOnRoad(Vehicle* newVehicle , Road * rd){
  newVehicle->on_road = (rd);
   //cout<<"Previous Size "<<(*rd).vehicles.size();
  // cout<<"type "<< newVehicle->type << " and id "<<newVehicle->id<<" for newVehicle"<<endl;
  (*rd).vehicles.insert(std::pair<int,Vehicle*>(newVehicle->id,newVehicle));
  (*rd).symbol_maps.insert(std::pair<char,Vehicle*>(newVehicle->symbol,newVehicle));
  //cout<<" Later Size "<<(*rd).vehicles.size()<<endl;
}

Vehicle* createVehicle( string type, string color , int length, int width, char symbol,int lane_no,int column_no, int max_speed, int max_acceleration, int lanechange_vertical_speed, int lanechange_horizontal_speed, int ini_vertical_velocity, int ini_horizontal_velocity,float lane_change_freq){
  Vehicle* newVehicle = new Vehicle;
  newVehicle->id = vehicle_id;
  vehicle_id++;//update the vehicle id
  newVehicle->type = type;
  newVehicle->color = color;
  newVehicle->length = length;
  newVehicle->symbol = symbol;
  newVehicle->width = width;

  newVehicle->pos[0] = lane_no;
  newVehicle->pos[1] = column_no;
  newVehicle->max_xspeed = max_speed;
  newVehicle->max_acceleration = max_acceleration;
  newVehicle->lanechange_vertical_speed = lanechange_vertical_speed;
  newVehicle->lanechange_horizontal_speed = lanechange_horizontal_speed;
  newVehicle->velocity[0] = ini_vertical_velocity;
  newVehicle->velocity[1] = ini_horizontal_velocity;
  newVehicle->lane_change_freq = lane_change_freq;
  return newVehicle;
}

template_vehicle find_in(string name, vector<template_vehicle> vec)
{
  template_vehicle temp;
  for(auto k=vec.begin();k!=vec.end();k++)
  {
    if(k->type.compare(name)==0)
      return *k;
  }
  return temp;
}
