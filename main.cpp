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

float getRandom();

class Road{
  int id;//
  int length;//
  int width;// make them of type final
  bool** road_matrix;
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
  int pos[2] = {0,0};//initial position (x,y) coordinates

  bool** road_matrix = on_road.road_matrix;

  void changeVelocity(){/*lane changing and overtaking not yet coded */
    int max_xvel = velocity[0]+max_acceleration;
    for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
        for(int j = 1; j <= max_xvel ; j++){
           if(road_matrix[j+pos[0]][i+pos[1]] == true){
             max_xvel = j-1;
             continue;
           }
        }
    }
   velocity[0]= max_xvel;
   velocity[1]= 0;
  }

  void changePosition(){// changing position in unit times
   pos[0] = pos[0]  + velocity[0];
   pos[1] = pos[1]  + velocity[1];
  }
}

void updateRoad(){//function to update road matrix

}

int main(char** argc, int argv){


}
