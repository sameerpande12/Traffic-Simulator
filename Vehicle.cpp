
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
#include "Vehicle.h"
#include "Road.h"
#include "common.h"
using namespace std;

class Road;

void Vehicle::changeVelocity(){
  /*lane changing and overtaking not yet coded */
  //  cout<<"entering changeVelocity"<<velocity[1]<<" "<<velocity[0]<<endl;
//  cout<<"Initial Velocity of "<<type<<" "<<id<<" "<<velocity[1]<<endl;
     float p = getRandom();
     //cout<<"Changing velocity for "<<type<<" and id "<<id<<endl;
     if( *(on_road->signal_color) == 'R'){

        p = 1;// to ensure no overtaking takes place when signal is red
     }
     int max_xvel = velocity[1]+max_acceleration;

     if(max_xvel > max_xspeed)max_xvel = max_xspeed;

     for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
         for(int j = 1; j <= max_xvel ; j++){
          if(j+pos[1]< on_road->length){/*i+pos[0]< on_road->width -> this part must be true*/

              if(j+pos[1]>= on_road->signal_pos && *(on_road->signal_color) == 'R' && pos[1]<on_road->signal_pos){
                max_xvel = j-1;
              //  cout<<"making "<<type<<" "<<id<<" velocity "<<max_xvel<<endl;
        //        cout<<"WAITING "<<type<<" "<<endl;

                continue;
              }

              if(i+pos[0]>=on_road->width)continue;
              if(on_road->road_matrix[i+pos[0]][j+pos[1]] != ' '){
                max_xvel = j-1;
                continue;
              }
         }
       }
     }


     if(p > lane_change_freq && max_xvel!=0){// no lane_changing when max_xel!=0 and p says not to change
        velocity[1]= max_xvel;
         velocity[0]= 0;
       //cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
     }
    else{//assuming for now, overtaking can take place at given speeds only. We need to account later on for variable overtaking speeds
       //vehicle will plan to overtake if at current time there is no vehicle that could with max_xspeed  cause a collision

        bool turn_right = true;

        if( pos[0]+lanechange_vertical_speed+ width <=on_road->width && pos[0]+lanechange_vertical_speed>=0 ){
            for(int i = pos[0]+lanechange_vertical_speed ; i< pos[0]+lanechange_vertical_speed+width;i++){
              for(int j = pos[1]-length+1+lanechange_horizontal_speed;j <= pos[1]+lanechange_horizontal_speed;j++){
                  if(j>=0 && j<on_road->length){
                    if(i>=on_road->width)continue;
                    if(on_road->road_matrix[i][j]!=' ' && on_road->road_matrix[i][j]!=symbol){
                      turn_right = false;

                      break;
                    }
                  }
              }
              if(turn_right==false)break;
            }

             if(turn_right){//checks if any other vehicle could crash into it assuming the max_xspeed of current vehicle

                 // pos[0]+lanechange_vertical_speed   to pos[0]+lanechange_vertical_speed + width-1
                /* for(int i = pos[0]+lanechange_vertical_speed; i< pos[0]+lanechange_vertical_speed+width;i++){
                   for(int j = 1;j<=max_xspeed;j++){ OVERTAKING RULE BY ASSUMING THAT the j-limit is determined only by max_speed of overtaking vehicle and not vehicles behind them
                     if(pos[1]-length+1+lanechange_horizontal_speed-max_xspeed>=0 && pos[1]-length+1+lanechange_horizontal_speed - max_xspeed < on_road->length){
                        if(on_road->road_matrix[i][pos[1]-length+1+lanechange_horizontal_speed - max_xspeed]!=' '){
                          to_overtake = false;
                          break;
                        }
                      }  //O(n) in total
                   }


                   if(to_overtake==false)break;
                 }*/

                map<char,Vehicle*>::iterator sym_iter = on_road->symbol_maps.begin();
                int x_left = pos[1]+lanechange_horizontal_speed -length + 1;
                int x_right = pos[1]+lanechange_horizontal_speed;

                int y_top = pos[0]+lanechange_vertical_speed;
                int y_bottom = y_top + width -1;


                for(sym_iter = on_road->symbol_maps.begin();sym_iter != on_road->symbol_maps.end();sym_iter++){
                  // if we reach here we can be sure that there is no overlap between sym_iter->second vehicle and (this) vehicle.
                   Vehicle* temp_veh = sym_iter->second;
                   int vx_left = temp_veh -> pos[1] - length + 1;
                   int vx_right = vx_left + temp_veh->velocity[1]+ temp_veh->max_acceleration;// we need to cover the entire rectangle possibilities: if temp_veh moves max fast or it halts: hence vx_left will be current position of left width
                   int vx_top = temp_veh -> pos[0];
                   int vx_bottom = vx_top + temp_veh -> width -1;

                   //cout<<"LEFT:"<<x_left<<" "<<x_right<<" "<<y_top<<" "<<y_bottom<<endl;
                   //cout<<vx_left<<" "<<vx_right<<" "<<vx_top<<" "<<vx_bottom<<endl;
                   if( (vx_left - x_left)*(vx_left - x_right)<=0 || (vx_right - x_left)*(vx_right - x_right)<=0 ){
                        if( (vx_top - y_top)*(vx_top - y_bottom) <=0 || (vx_bottom - y_top)*(vx_bottom-y_bottom)<=0){
                          turn_right = false;

                          break;
                        }
                   }
                }

             }

         }
        else turn_right = false;



        bool turn_left = true;
        if( pos[0] - lanechange_vertical_speed + width<=on_road->width && pos[0]-lanechange_vertical_speed>=0){
            for(int i = pos[0]-lanechange_vertical_speed;i<pos[0]-lanechange_vertical_speed+width;i++){
              for(int j = pos[1]-length+1+lanechange_horizontal_speed;j <= pos[1]+lanechange_horizontal_speed;j++){
                  if(j>=0 && j<on_road->length){
                    if(i>=on_road->width)continue;
                    if(on_road->road_matrix[i][j]!=' ' && on_road->road_matrix[i][j]!=symbol){
                      turn_left = false;

                      break;
                    }
                  }
              }
              if(turn_left==false)break;
            }

           if(turn_left){
                map<char,Vehicle*>::iterator sym_iter = on_road->symbol_maps.begin();
                int x_left = pos[1]+lanechange_horizontal_speed -length + 1;
                int x_right = pos[1]+lanechange_horizontal_speed;

                int y_top = pos[0]-lanechange_vertical_speed;
                int y_bottom = y_top + width -1;


                for(sym_iter = on_road->symbol_maps.begin();sym_iter != on_road->symbol_maps.end();sym_iter++){
                  // if we reach here we can be sure that there is no overlap between sym_iter->second vehicle and (this) vehicle.
                   Vehicle* temp_veh = sym_iter->second;
                   int vx_left = temp_veh -> pos[1] - length + 1;
                   int vx_right = vx_left + temp_veh->velocity[1]+ temp_veh->max_acceleration;// we need to cover the entire rectangle possibilities: if temp_veh moves max fast or it halts: hence vx_left will be current position of left width
                   int vx_top = temp_veh -> pos[0];
                   int vx_bottom = vx_top + temp_veh -> width -1;

                   if( (vx_left - x_left)*(vx_left - x_right)<=0 || (vx_right - x_left)*(vx_right - x_right)<=0 ){
                        if( (vx_top - y_top)*(vx_top - y_bottom) <=0 || (vx_bottom - y_top)*(vx_bottom-y_bottom)<=0){
                          turn_left = false;

                          break;
                        }
                   }
                }

             }

        }
        else turn_left = false;


          float direction_prob = getRandom();
          if(direction_prob > 0.5){
            if(turn_right){turn_left = false;}
          }
          else{
            if(turn_left){turn_right = false;}
          }

        if(turn_right && !(pos[1]< on_road->signal_pos  && pos[1]+lanechange_horizontal_speed>=on_road->signal_pos ) ) {
          velocity[1]=lanechange_horizontal_speed;
          velocity[0]=lanechange_vertical_speed;
        }
        else if(turn_left && !(pos[1]< on_road->signal_pos  && pos[1]+lanechange_horizontal_speed>=on_road->signal_pos )){
          velocity[1]=lanechange_horizontal_speed;
          velocity[0]=-lanechange_vertical_speed;
        }
        else{
          velocity[1] = max_xvel;
          velocity[0]=0;
        }
      //  if(to_overtake)cout<<"Overtaking by "<<type<<endl;
      //    else cout<<"NO overtaking "<<type<<endl;

      //  cout<<type<<" velocity[0]:"<<velocity[0]<<" velocity[1]:"<<velocity[1]<<endl;
    }
//  cout<<"Changed Velocity of "<<type<<" "<<id<<" "<<velocity[1]<<endl;
  // cout<<"leaving changeVelocity: "<<velocity[1]<<" "<<velocity[0]<<endl;
}

void Vehicle::changePosition(){
  // changing position in unit times
  //cout<<"entering changePosition"<<velocity[1]<<" "<<velocity[0]<<endl;
   pos[0] = pos[0]  + velocity[0];
   pos[1] = pos[1]  + velocity[1];
   //cout<<"leaving changePosition: "<<endl;
}
