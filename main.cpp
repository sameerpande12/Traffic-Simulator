/*
Axes:- Right -> Positive,
       Down -> Positive

(i,j) for roads-> indexing pattern
----------------------
0,0     0,1       0,2
1,0     1,1       1,2
-----------------------
references- https://www.geeksforgeeks.org/stdany-class-in-c/
https://en.cppreference.com/w/cpp/numeric/random/uniform_real_distribution
*/
#include<any>
#include<cstring>
#include<string>
#include <iostream>
#include<map>
#include<vector>
#include<random>
using namespace std;

std::random_device random_gen;
std::mt19937 gen(random_gen());
std::uniform_real_distribution<>dis(0.0,1.0);


float getRandom(){
  return (float) dis(gen);
};//returns a float between 0 to 1 in uniform distribution
class Vehicle;
class Road{
public:
  int id;//
  int length;//
  int width;// make them of type final
  bool** road_matrix;
  map<int,Vehicle> vehicles;
};

class Vehicle{
public:

 //existential parameters
  int id;//ids should be unique for all the vehicles
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
     int max_xvel = velocity[0]+max_acceleration;
     for(int i = 0; i<width ;i++){//ensures that driver takes maximum velocity possible so as to avoid collision
         for(int j = 1; j <= max_xvel ; j++){
            if(road_matrix[i+pos[0]][j+pos[1]] == true){
              max_xvel = j-1;
              continue;
            }
         }
     }
     if(p > overtake_freq){ // no overtaking
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
};

void updateRoad(Road rd){//function to update road matrix
  map<int,Vehicle> vehicles = rd.vehicles;
  for(int i = 0;i<rd.width;i++)
   for(int j = 0;j<rd.length;j++)rd.road_matrix[i][j]=false;

  map<int,Vehicle>::iterator iter = vehicles.begin();
  for(iter = vehicles.begin();iter!=vehicles.end();iter++)iter->second.changeVelocity();

  iter = vehicles.begin();

  std::vector<int> ids_to_remove;
  for(iter = vehicles.begin();iter!=vehicles.end();iter++){
    iter->second.changePosition();
    for(int i = 0;i<iter->second.width;i++){
      for(int j = 0;j<iter->second.length;j++){
        if(iter->second.pos[1]>=j && iter->second.pos[1]-j<=rd.length){
          rd.road_matrix[i+iter->second.pos[0]][iter->second.pos[1]-j]=true;
        }
      }
    }
    if(iter->second.pos[1]-iter->second.length+1>=rd.length){
         ids_to_remove.push_back(iter->first);
    }
  }

  vector<int>::iterator vec_iter = ids_to_remove.begin();
  for(vec_iter = ids_to_remove.begin();vec_iter<ids_to_remove.end();vec_iter++){
    vehicles.erase(*vec_iter);
  }
  //updates the road by one unit time
}

int main(int argc, char** argv){


}
