/*
Axes:- Right -> Positive,
       Down -> Positive

(i,j) for roads-> indexing pattern
----------------------
0,0     0,1       0,2
1,0     1,1       1,2
-----------------------
references- https://www.geeksforgeeks.org/stdany-class-in-c/
*/
#include<any>
#include<cstring>
#include<string>
#include <iostream>
#include<vector>
using namespace std;

float getRandom();

class Road{
  int id;//
  int length;//
  int width;// make them of type final
  bool** road_matrix;
  Vector<Vehicle> vehicles;
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
  float overtake_freq = 0.1;
  int overtake_horizontal_speed = 1;
  int overtake_vertical_speed = 1;
  //overtaking done only from right side of vehicle to be overtaken
  bool changing_lane = false;
  /*probability of overtaking given overtaking is possible*/
  /*we define overtaking is possible when a smaller vehicle can shift to a lane(s) on which no trace of the vehicle to be overtaken is found*/
  /*for time being assume instantaneous breaking is possible:- Scenario is almost true for indian drivers xD*/

  int velocity[2]={0,0};//velocity[0] is x_velocity and velocity[1] is y_velocity
  int lane_no;//represents lane on which top right corner of vehicle is
  Road on_road;//road on which vehicle is moving
  int pos[2] = {0,0};

  bool** road_matrix = on_road.road_matrix;

  void changeVelocity(){/*lane changing and overtaking not yet coded */
     float p = getRandom();
     if(p > overtake_freq){ // no overtaking

        int max_xvel = velocity[0]+max_acceleration;
        for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
            for(int j = 1; j <= max_xvel ; j++){
               if(road_matrix[i+pos[0]][j+pos[1]] == true){
                 max_xvel = j-1;
                 continue;
               }
            }
        }
       velocity[1]= max_xvel;
       velocity[0]= 0;
    }
    else{//assuming for now, overtaking can take place at given speeds only. We need to account later on for variable overtaking speeds
      bool to_overtake = true;
      if(pos[1]+overtake_vertical_speed<on_road.width && pos[1]+overtake_vertical_speed>=0){
       for(int i = 1;i<=overtake_horizontal_speed;i++){
          for(int j =1; j<=overtake_vertical_speed;j++){
            if(road_matrix[j+pos[0]][pos[1]+i]==true){
              to_overtake = false;
              break;
            }
          }
          if(to_overtake==false)break;
       }
      }
      else to_overtake = false;

      if(to_overtake){
        velocity[1]=overtake_horizontal_speed;
        velocity[0]=overtake_vertical_speed;
      }
      else{
        velocity[1] = max_xvel;
        velocity[0]=0;
      }

    }

  }

  void changePosition(){// changing position in unit times
   pos[0] = pos[0]  + velocity[0];
   pos[1] = pos[1]  + velocity[1];
  }
}

void updateRoad(Road rd){//function to update road matrix
  Vector<Vehicle> vehicles = rd.vehicles;
  for(int i = 0;i<rd.width;i++)
   for(int j = 0;j<rd.length;j++)rd.road_matrix[i][j]=false;

  Vector<Vehicle>::iterator iter = vehicles.begin();
  for(iter = vehicles.begin();iter<vehicles.end();iter++)*iter.changeVelocity();

  iter = vehicles.begin();
  for(iter = vehicles.begin();iter<vehicles.end();iter++){
    *iter.changePosition();
    for(int i = 0;i<*iter.width;i++){
      for(int j = 0;j<*iter.length;j++){
        if(*iter.position[1]>=j){
          rd.road_matrix[i+*iter.position[0]][*iter.position[1]-j]=true;
        }
      }
    }
  }
  //updates the road by one unit time
}

int main(char** argc, int argv){


}
